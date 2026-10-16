/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** Input class
*/

#pragma once

#include "InputTypes.hpp"
#include "Types.hpp"
#include <optional>

namespace Backend {

    class Window;

    /**
     * Keyboard, mouse and gamepad state for the
     * current frame (events are polled by
     * `Window::endFrame()`).
     *
     * "Pressed" / "Released" are true only on the
     * frame the transition happened, "Down" while held.
     * Out-of-range enum values or gamepad indexes
     * return false / 0.
     */
    class Input {
        const Window& m_Window;

        public:
            explicit Input(const Window& window) noexcept;

            bool isDown(Key key) const noexcept;
            bool isPressed(Key key) const noexcept;
            bool isReleased(Key key) const noexcept;

            /**
             * Pops the next key pressed this frame from the
             * queue, nullopt once empty. Keys outside `Key`
             * are skipped. Meant for key-remapping screens.
             */
            std::optional<Key> nextPressed() noexcept;

            bool isDown(MouseButton button) const noexcept;
            bool isPressed(MouseButton button) const noexcept;
            bool isReleased(MouseButton button) const noexcept;
            Vec2 mousePosition() const noexcept;    // Virtual coordinates
            float mouseWheel() const noexcept;      // Vertical wheel movement this frame

            bool isGamepadAvailable(int gamepad) const noexcept;
            bool isDown(int gamepad, GamepadButton button) const noexcept;
            bool isPressed(int gamepad, GamepadButton button) const noexcept;
            bool isReleased(int gamepad, GamepadButton button) const noexcept;
            /**
             * Sticks are in [-1, 1], triggers in [0, 1]
             * (0 when released). 0 if the gamepad is unavailable.
             */
            float axis(int gamepad, GamepadAxis axis) const noexcept;
    };
}
