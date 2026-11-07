/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** player preferences
*/

#include "Preferences.hpp"
#include <cctype>
#include <charconv>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <system_error>


namespace {
    using RType::Game::Action;
    using RType::Game::Preferences;
    using RType::Game::Slot;

    constexpr float PERCENT = 100.0F;
    constexpr std::string_view NO_BINDING = "None";

    std::string_view trim(std::string_view text) noexcept
    {
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.front())))
            text.remove_prefix(1);
        while (!text.empty() && std::isspace(static_cast<unsigned char>(text.back())))
            text.remove_suffix(1);
        return text;
    }

    std::string lower(std::string_view text)
    {
        std::string result(text);

        for (char& c : result)
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        return result;
    }

    std::string_view slotName(Slot slot) noexcept
    {
        return slot == Slot::Keyboard ? "keyboard" : "gamepad";
    }

    std::string bindKey(Action action, Slot slot)
    {
        return "bind." + lower(RType::Game::actionName(action)) + "." + std::string(slotName(slot));
    }

    int toPercent(float volume) noexcept
    {
        return static_cast<int>(std::lround(volume * PERCENT));
    }

    void readVolume(std::string_view value, int& volume) noexcept
    {
        int result = 0;
        auto [end, error] = std::from_chars(value.data(), value.data() + value.size(), result);

        if (error == std::errc() && end == value.data() + value.size() && result >= 0 && result <= Preferences::VOLUME_MAX)
            volume = result;
    }

    std::optional<RType::Render::Binding> readBinding(std::string_view value, Slot slot) noexcept
    {
        if (slot == Slot::Gamepad) {
            if (auto button = RType::Render::parseGamepadButton(value))
                return *button;
            return std::nullopt;
        }
        if (auto key = RType::Render::parseKey(value))
            return *key;
        if (auto button = RType::Render::parseMouseButton(value))
            return *button;
        return std::nullopt;
    }

    // Returns false if the key is not a binding
    bool readBindingLine(Preferences& preferences, std::string_view key, std::string_view value)
    {
        for (Action action : RType::Game::ACTIONS) {
            for (Slot slot : { Slot::Keyboard, Slot::Gamepad }) {
                if (key != bindKey(action, slot))
                    continue;
                if (value == NO_BINDING) {
                    if (auto old = RType::Game::slotBinding(preferences.controls, action, slot))
                        preferences.controls.unbind(action, *old);
                } else if (auto binding = readBinding(value, slot)) {
                    RType::Game::rebind(preferences.controls, action, slot, *binding);
                }
                return true;
            }
        }
        return false;
    }

    void readLine(Preferences& preferences, std::string_view line)
    {
        std::size_t equal = line.find('=');

        if (equal == std::string_view::npos)
            return;

        std::string_view key = trim(line.substr(0, equal));
        std::string_view value = trim(line.substr(equal + 1));

        if (key == "volume.master")
            readVolume(value, preferences.master);
        else if (key == "volume.music")
            readVolume(value, preferences.music);
        else if (key == "volume.effects")
            readVolume(value, preferences.effects);
        else if (key == "fullscreen" && (value == "true" || value == "false"))
            preferences.fullscreen = value == "true";
        else if (key == "accessibility.high_contrast" && (value == "true" || value == "false"))
            preferences.highContrast = value == "true";
        else
            readBindingLine(preferences, key, value);
    }

    std::optional<std::filesystem::path> environmentPath(const char* name)
    {
#ifdef _MSC_VER
    #pragma warning(suppress : 4996)  // getenv is fine here: read once, never stored
#endif
        const char* value = std::getenv(name);

        if (value == nullptr || *value == '\0')
            return std::nullopt;
        return std::filesystem::path(value);
    }
}


auto RType::Game::currentPreferences(const Render::Audio& audio, const Render::Window& window, const Controls& controls,
                                     const DisplayOptions& display) -> Preferences
{
    return Preferences{
        .master = toPercent(audio.volume(Render::Channel::Master)),
        .music = toPercent(audio.volume(Render::Channel::Music)),
        .effects = toPercent(audio.volume(Render::Channel::Effects)),
        .fullscreen = window.isFullscreen(),
        .highContrast = display.highContrast,
        .controls = controls,
    };
}

void RType::Game::applyPreferences(const Preferences& preferences, Render::Audio& audio, Render::Window& window, Controls& controls,
                                   DisplayOptions& display)
{
    int gamepad = controls.gamepad();

    audio.setVolume(Render::Channel::Master, static_cast<float>(preferences.master) / PERCENT);
    audio.setVolume(Render::Channel::Music, static_cast<float>(preferences.music) / PERCENT);
    audio.setVolume(Render::Channel::Effects, static_cast<float>(preferences.effects) / PERCENT);
    if (preferences.fullscreen != window.isFullscreen())
        window.toggleFullscreen();
    controls = preferences.controls;
    controls.setGamepad(gamepad);
    display.highContrast = preferences.highContrast;
}

std::string RType::Game::toText(const Preferences& preferences)
{
    std::ostringstream text;

    text << "volume.master=" << preferences.master << '\n'
         << "volume.music=" << preferences.music << '\n'
         << "volume.effects=" << preferences.effects << '\n'
         << "fullscreen=" << (preferences.fullscreen ? "true" : "false") << '\n'
         << "accessibility.high_contrast=" << (preferences.highContrast ? "true" : "false") << '\n';

    for (Action action : ACTIONS) {
        for (Slot slot : { Slot::Keyboard, Slot::Gamepad }) {
            std::optional<Render::Binding> binding = slotBinding(preferences.controls, action, slot);

            text << bindKey(action, slot) << '=' << (binding ? bindingName(*binding) : NO_BINDING) << '\n';
        }
    }
    return text.str();
}

auto RType::Game::fromText(std::string_view text, Preferences defaults) -> Preferences
{
    while (!text.empty()) {
        std::size_t end = text.find('\n');
        std::string_view line = text.substr(0, end);

        readLine(defaults, line);
        text.remove_prefix(end == std::string_view::npos ? text.size() : end + 1);
    }
    return defaults;
}

auto RType::Game::preferencesPath() -> std::optional<std::filesystem::path>
{
#ifdef _WIN32
    std::optional<std::filesystem::path> base = environmentPath("APPDATA");
#else
    std::optional<std::filesystem::path> base = environmentPath("XDG_CONFIG_HOME");

    if (!base)
        if (std::optional<std::filesystem::path> home = environmentPath("HOME"))
            base = *home / ".config";
#endif
    if (!base)
        return std::nullopt;
    return *base / "r-type" / "settings.cfg";
}

auto RType::Game::loadPreferences(Preferences defaults) -> Preferences
{
    std::optional<std::filesystem::path> path = preferencesPath();
    std::error_code error;

    if (!path || !std::filesystem::is_regular_file(*path, error) || std::filesystem::file_size(*path, error) > Preferences::FILE_SIZE_MAX)
        return defaults;

    std::ifstream file(*path, std::ios::binary);
    std::string text((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    return fromText(text, std::move(defaults));
}

bool RType::Game::savePreferences(const Preferences& preferences)
{
    std::optional<std::filesystem::path> path = preferencesPath();
    std::error_code error;

    if (!path) {
        std::cerr << "settings not saved: no config folder (HOME / APPDATA not set)" << std::endl;
        return false;
    }
    std::filesystem::create_directories(path->parent_path(), error);

    std::ofstream file(*path, std::ios::binary | std::ios::trunc);

    file << toText(preferences);
    if (!file) {
        std::cerr << "settings not saved: cannot write " << path->string() << std::endl;
        return false;
    }
    return true;
}
