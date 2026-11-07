/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** human readable key names
*/

#include "../InputTypes.hpp"
#include <array>
#include <cstddef>
#include <optional>

namespace {

    // Indexed by RType::Render::Key
    constexpr auto KEY_NAMES = std::to_array<std::string_view>({
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
        "Up", "Down", "Left", "Right",
        "Space", "Enter", "Escape", "Backspace", "Tab",
        "Insert", "Delete", "Home", "End", "Page Up", "Page Down",
        "Left Shift", "Right Shift", "Left Control", "Right Control", "Left Alt", "Right Alt",
        "-", "=", ",", ".", "/", ";", "'",
        "[", "]", "\\", "`",
        "Keypad 0", "Keypad 1", "Keypad 2", "Keypad 3", "Keypad 4",
        "Keypad 5", "Keypad 6", "Keypad 7", "Keypad 8", "Keypad 9",
        "Keypad +", "Keypad -", "Keypad *", "Keypad /", "Keypad Enter", "Keypad .",
    });
    static_assert(KEY_NAMES.size() == static_cast<std::size_t>(RType::Render::Key::Count));

    // Indexed by RType::Render::MouseButton
    constexpr auto MOUSE_BUTTON_NAMES = std::to_array<std::string_view>({
        "Mouse Left", "Mouse Right", "Mouse Middle",
    });
    static_assert(MOUSE_BUTTON_NAMES.size() == static_cast<std::size_t>(RType::Render::MouseButton::Count));

    // Indexed by RType::Render::GamepadButton
    constexpr auto GAMEPAD_BUTTON_NAMES = std::to_array<std::string_view>({
        "D-pad Up", "D-pad Right", "D-pad Down", "D-pad Left",
        "Y", "B", "A", "X",
        "Left Bumper", "Left Trigger", "Right Bumper", "Right Trigger",
        "Back", "Guide", "Start", "Left Stick", "Right Stick",
    });
    static_assert(GAMEPAD_BUTTON_NAMES.size() == static_cast<std::size_t>(RType::Render::GamepadButton::Count));

    template <typename E, std::size_t N>
    std::string_view nameOf(const std::array<std::string_view, N>& names, E value) noexcept
    {
        auto index = static_cast<std::size_t>(value);

        return index < names.size() ? names[index] : "Unknown";
    }

    template <typename E, std::size_t N>
    std::optional<E> parse(const std::array<std::string_view, N>& names, std::string_view name) noexcept
    {
        for (std::size_t i = 0; i < names.size(); ++i)
            if (names[i] == name)
                return static_cast<E>(i);
        return std::nullopt;
    }
}

std::string_view RType::Render::keyName(Key key) noexcept
{
    return nameOf(KEY_NAMES, key);
}

std::string_view RType::Render::mouseButtonName(MouseButton button) noexcept
{
    return nameOf(MOUSE_BUTTON_NAMES, button);
}

std::string_view RType::Render::gamepadButtonName(GamepadButton button) noexcept
{
    return nameOf(GAMEPAD_BUTTON_NAMES, button);
}

auto RType::Render::parseKey(std::string_view name) noexcept -> std::optional<Key>
{
    return parse<Key>(KEY_NAMES, name);
}

auto RType::Render::parseMouseButton(std::string_view name) noexcept -> std::optional<MouseButton>
{
    return parse<MouseButton>(MOUSE_BUTTON_NAMES, name);
}

auto RType::Render::parseGamepadButton(std::string_view name) noexcept -> std::optional<GamepadButton>
{
    return parse<GamepadButton>(GAMEPAD_BUTTON_NAMES, name);
}
