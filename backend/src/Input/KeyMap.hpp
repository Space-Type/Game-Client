/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Private header for
** backend <-> raylib input mappings
*/

#pragma once

// Backend headers must come before raylib.h: raylib defines
// color macros (BLACK, WHITE...) that clash with Backend::Colors
#include <backend/InputTypes.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <optional>
#include <type_traits>
#include <raylib.h>

namespace Backend::Detail {

    /** Number of values of an input enum (its `Count` member) */
    template <typename E>
    inline constexpr std::size_t COUNT_OF = static_cast<std::size_t>(E::Count);

    /** Index of an input enum value, COUNT_OF<E> if out of range */
    template <typename E>
    constexpr std::size_t indexOf(E value) noexcept
    {
        auto index = static_cast<std::size_t>(static_cast<std::underlying_type_t<E>>(value));

        return index < COUNT_OF<E> ? index : COUNT_OF<E>;
    }

    // Indexed by Backend::Key
    inline constexpr auto KEYS = std::to_array<int>({
        KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J, KEY_K, KEY_L, KEY_M,
        KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
        KEY_ZERO, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR,
        KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE,
        KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6,
        KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12,
        KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT,
        KEY_SPACE, KEY_ENTER, KEY_ESCAPE, KEY_BACKSPACE, KEY_TAB,
        KEY_INSERT, KEY_DELETE, KEY_HOME, KEY_END, KEY_PAGE_UP, KEY_PAGE_DOWN,
        KEY_LEFT_SHIFT, KEY_RIGHT_SHIFT, KEY_LEFT_CONTROL, KEY_RIGHT_CONTROL, KEY_LEFT_ALT, KEY_RIGHT_ALT,
        KEY_MINUS, KEY_EQUAL, KEY_COMMA, KEY_PERIOD, KEY_SLASH, KEY_SEMICOLON, KEY_APOSTROPHE,
        KEY_LEFT_BRACKET, KEY_RIGHT_BRACKET, KEY_BACKSLASH, KEY_GRAVE,
        KEY_KP_0, KEY_KP_1, KEY_KP_2, KEY_KP_3, KEY_KP_4,
        KEY_KP_5, KEY_KP_6, KEY_KP_7, KEY_KP_8, KEY_KP_9,
        KEY_KP_ADD, KEY_KP_SUBTRACT, KEY_KP_MULTIPLY, KEY_KP_DIVIDE, KEY_KP_ENTER, KEY_KP_DECIMAL,
    });
    static_assert(KEYS.size() == COUNT_OF<Key>);

    // Indexed by Backend::MouseButton
    inline constexpr auto MOUSE_BUTTONS = std::to_array<int>({
        MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_MIDDLE,
    });
    static_assert(MOUSE_BUTTONS.size() == COUNT_OF<MouseButton>);

    // Indexed by Backend::GamepadButton
    inline constexpr auto GAMEPAD_BUTTONS = std::to_array<int>({
        GAMEPAD_BUTTON_LEFT_FACE_UP,        // DpadUp
        GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // DpadRight
        GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // DpadDown
        GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // DpadLeft
        GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Y
        GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // B
        GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // A
        GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // X
        GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // LeftBumper
        GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // LeftTrigger
        GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // RightBumper
        GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // RightTrigger
        GAMEPAD_BUTTON_MIDDLE_LEFT,         // Back
        GAMEPAD_BUTTON_MIDDLE,              // Guide
        GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Start
        GAMEPAD_BUTTON_LEFT_THUMB,          // LeftThumb
        GAMEPAD_BUTTON_RIGHT_THUMB,         // RightThumb
    });
    static_assert(GAMEPAD_BUTTONS.size() == COUNT_OF<GamepadButton>);

    // Indexed by Backend::GamepadAxis
    inline constexpr auto GAMEPAD_AXES = std::to_array<int>({
        GAMEPAD_AXIS_LEFT_X, GAMEPAD_AXIS_LEFT_Y,
        GAMEPAD_AXIS_RIGHT_X, GAMEPAD_AXIS_RIGHT_Y,
        GAMEPAD_AXIS_LEFT_TRIGGER, GAMEPAD_AXIS_RIGHT_TRIGGER,
    });
    static_assert(GAMEPAD_AXES.size() == COUNT_OF<GamepadAxis>);

    /** raylib key code, KEY_NULL if out of range */
    constexpr int toRaylib(Key key) noexcept
    {
        std::size_t index = indexOf(key);

        return index < KEYS.size() ? KEYS[index] : KEY_NULL;
    }

    /** raylib mouse button, -1 if out of range */
    constexpr int toRaylib(MouseButton button) noexcept
    {
        std::size_t index = indexOf(button);

        return index < MOUSE_BUTTONS.size() ? MOUSE_BUTTONS[index] : -1;
    }

    /** raylib gamepad button, GAMEPAD_BUTTON_UNKNOWN if out of range */
    constexpr int toRaylib(GamepadButton button) noexcept
    {
        std::size_t index = indexOf(button);

        return index < GAMEPAD_BUTTONS.size() ? GAMEPAD_BUTTONS[index] : GAMEPAD_BUTTON_UNKNOWN;
    }

    /** raylib gamepad axis, -1 if out of range */
    constexpr int toRaylib(GamepadAxis axis) noexcept
    {
        std::size_t index = indexOf(axis);

        return index < GAMEPAD_AXES.size() ? GAMEPAD_AXES[index] : -1;
    }

    /**
     * Normalizes a raw raylib axis value: sticks are clamped
     * to [-1, 1], triggers are mapped from [-1, 1] (-1 = released)
     * to [0, 1]. NaN and invalid axes give 0
     */
    inline float normalizeAxis(GamepadAxis axis, float raw) noexcept
    {
        if (std::isnan(raw) || indexOf(axis) >= COUNT_OF<GamepadAxis>)
            return 0.0F;
        if (axis == GamepadAxis::LeftTrigger || axis == GamepadAxis::RightTrigger)
            return std::clamp((raw + 1.0F) / 2.0F, 0.0F, 1.0F);
        return std::clamp(raw, -1.0F, 1.0F);
    }

    /** Backend key of a raylib key code, nullopt if unmapped */
    constexpr std::optional<Key> fromRaylibKey(int key) noexcept
    {
        if (key == KEY_NULL)
            return std::nullopt;
        for (std::size_t i = 0; i < KEYS.size(); i++)
            if (KEYS[i] == key)
                return static_cast<Key>(i);
        return std::nullopt;
    }
}
