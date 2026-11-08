/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Input class
*/

#pragma once

#include "../InputTypes.hpp"
#include "../Types.hpp"
#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>

namespace RType::Render {

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
     *
     * A gamepad is only usable with a mapping (SDL /
     * GLFW format) that says which button is which:
     * a joystick without one sends nothing.
     */
    class Input {
        const Window& m_Window;

        public:
            static constexpr int MAX_GAMEPADS = 4;
            static constexpr std::size_t MAPPINGS_FILE_MAX = 4 * 1024 * 1024;  // the whole SDL database is ~600 KiB

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

            /**
             * Pops the next character typed this frame from
             * the queue (Unicode code point, following the
             * keyboard layout), nullopt once empty.
             */
            std::optional<char32_t> nextChar() noexcept;

            bool isDown(MouseButton button) const noexcept;
            bool isPressed(MouseButton button) const noexcept;
            bool isReleased(MouseButton button) const noexcept;
            Vec2D mousePosition() const noexcept;    // Virtual coordinates
            float mouseWheel() const noexcept;      // Vertical wheel movement this frame

            /**
             * Adds gamepad mappings, one per line in the
             * SDL_GameControllerDB format (lines for other
             * platforms are skipped). A later mapping replaces
             * an earlier one for the same controller. Needs
             * the window to be open; false if nothing was read.
             */
            bool addGamepadMappings(const std::string& mappings) noexcept;

            // Same from a text file: false if it is missing, unreadable or bigger than MAPPINGS_FILE_MAX
            bool loadGamepadMappings(const std::filesystem::path& file);

            // Connected and mapped: its buttons and axes can be read
            bool isGamepadAvailable(int gamepad) const noexcept;

            // Connected, but without a mapping for it (unknown controller)
            bool isUnmappedJoystick(int gamepad) const noexcept;

            // Name the device gives ("Xbox Wireless Controller"), empty if nothing is connected there
            std::string gamepadName(int gamepad) const;

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
