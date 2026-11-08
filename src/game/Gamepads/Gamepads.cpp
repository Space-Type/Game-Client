/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** gamepad mappings and choice
*/

#include "Gamepads.hpp"
#include "../Preferences/Preferences.hpp"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>


auto RType::Game::connectedGamepads(const Render::Input& input) -> std::vector<ConnectedGamepad>
{
    std::vector<ConnectedGamepad> gamepads;

    for (int i = 0; i < Render::Input::MAX_GAMEPADS; ++i)
        if (input.isGamepadAvailable(i))
            gamepads.push_back({ .index = i, .name = input.gamepadName(i) });
    return gamepads;
}

std::vector<std::string> RType::Game::unknownControllers(const Render::Input& input)
{
    std::vector<std::string> names;

    for (int i = 0; i < Render::Input::MAX_GAMEPADS; ++i)
        if (input.isUnmappedJoystick(i))
            names.push_back(input.gamepadName(i));
    return names;
}

int RType::Game::chooseGamepad(const std::vector<ConnectedGamepad>& gamepads, std::string_view preferred) noexcept
{
    if (gamepads.empty())
        return -1;

    auto chosen = std::ranges::find(gamepads, preferred, &ConnectedGamepad::name);

    return !preferred.empty() && chosen != gamepads.end() ? chosen->index : gamepads.front().index;
}

std::string RType::Game::nextGamepadChoice(const std::vector<ConnectedGamepad>& gamepads, std::string_view current)
{
    if (gamepads.empty())
        return {};
    if (current.empty())
        return gamepads.front().name;

    auto it = std::ranges::find(gamepads, current, &ConnectedGamepad::name);

    // The last one goes back to "first found"; a name no longer connected goes to the first gamepad
    if (it == gamepads.end())
        return gamepads.front().name;
    return std::next(it) == gamepads.end() ? std::string() : std::next(it)->name;
}

auto RType::Game::playerMappingsPath() -> std::optional<std::filesystem::path>
{
    std::optional<std::filesystem::path> settings = preferencesPath();

    if (!settings)
        return std::nullopt;
    return settings->parent_path() / "gamecontrollerdb.txt";
}

void RType::Game::loadGamepadMappings(Render::Input& input)
{
    if (!input.loadGamepadMappings(Render::applicationPath("gamecontrollerdb.txt")))
        std::cerr << "no gamepad database next to the client: only GLFW's built-in mappings" << std::endl;

    if (std::optional<std::filesystem::path> player = playerMappingsPath())
        input.loadGamepadMappings(*player);  // optional: most players never need it

#ifdef _MSC_VER
    #pragma warning(suppress : 4996)  // getenv is fine here: read once, never stored
#endif
    const char* fromEnvironment = std::getenv("SDL_GAMECONTROLLERCONFIG");

    if (fromEnvironment != nullptr)
        input.addGamepadMappings(fromEnvironment);
}
