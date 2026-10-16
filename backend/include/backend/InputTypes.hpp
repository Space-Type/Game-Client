/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** input device enums
*/

#pragma once

#include <cstdint>
#include <string_view>

namespace Backend {

    /**
     * Keyboard keys, identified by their physical
     * position on a US QWERTY layout (e.g. `Key::W`
     * is `Z` on an AZERTY keyboard).
     *
     * Use `keyName()` to display a key to the user.
     */
    enum class Key : std::uint8_t {
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
        Up, Down, Left, Right,
        Space, Enter, Escape, Backspace, Tab,
        Insert, Delete, Home, End, PageUp, PageDown,
        LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
        Minus, Equal, Comma, Period, Slash, Semicolon, Apostrophe,
        LeftBracket, RightBracket, Backslash, Grave,
        Kp0, Kp1, Kp2, Kp3, Kp4, Kp5, Kp6, Kp7, Kp8, Kp9,
        KpAdd, KpSubtract, KpMultiply, KpDivide, KpEnter, KpDecimal,
        Count,  // Not a key: number of keys
    };

    enum class MouseButton : std::uint8_t {
        Left,
        Right,
        Middle,
        Count,  // Not a button: number of buttons
    };

    // Xbox naming, PlayStation equivalent in comments
    enum class GamepadButton : std::uint8_t {
        DpadUp,
        DpadRight,
        DpadDown,
        DpadLeft,
        Y,              // Triangle
        B,              // Circle
        A,              // Cross
        X,              // Square
        LeftBumper,     // L1
        LeftTrigger,    // L2
        RightBumper,    // R1
        RightTrigger,   // R2
        Back,           // Select
        Guide,          // PS
        Start,
        LeftThumb,      // L3
        RightThumb,     // R3
        Count,  // Not a button: number of buttons
    };

    enum class GamepadAxis : std::uint8_t {
        LeftX,
        LeftY,
        RightX,
        RightY,
        LeftTrigger,
        RightTrigger,
        Count,  // Not an axis: number of axes
    };

    /**
     * Axis used as a digital input: active while
     * the axis value is beyond `threshold`
     * (value >= threshold if threshold > 0,
     * value <= threshold if threshold < 0).
     */
    struct AxisBinding {
        GamepadAxis axis = GamepadAxis::LeftX;
        float threshold = 0.5F;

        constexpr bool operator==(const AxisBinding&) const noexcept = default;
    };

    /**
     * Human readable key name (e.g. "A", "Left Shift", "F5").
     * Returns "Unknown" for `Key::Count` or invalid values.
     */
    std::string_view keyName(Key key) noexcept;
}
