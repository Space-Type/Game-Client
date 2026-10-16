/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Source file for
** human readable key names
*/

#include <backend/InputTypes.hpp>
#include <array>
#include <cstddef>

namespace {

    // Indexed by Backend::Key
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
    static_assert(KEY_NAMES.size() == static_cast<std::size_t>(Backend::Key::Count));
}

std::string_view Backend::keyName(Key key) noexcept
{
    auto index = static_cast<std::size_t>(key);

    return index < KEY_NAMES.size() ? KEY_NAMES[index] : "Unknown";
}
