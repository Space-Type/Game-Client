/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** ActionMap class
*/

#pragma once

#include "Concepts.hpp"
#include "InputTypes.hpp"
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace Backend {

    using Binding = std::variant<Key, MouseButton, GamepadButton, AxisBinding>;

    /**
     * Maps game-defined actions (any enum) to one or
     * more input bindings, so controls can be remapped
     * at runtime (accessibility).
     *
     * Gamepad bindings read the gamepad selected with
     * `setGamepad()` (0 by default).
     *
     * Axis bindings only count for `isDown()`: they have
     * no pressed / released transition.
     */
    template <typename A>
    requires (std::is_enum_v<A>)
    class ActionMap {
        std::unordered_map<A, std::vector<Binding>> m_Bindings;
        int m_Gamepad;

        public:
            ActionMap() noexcept
                : m_Bindings()
                , m_Gamepad(0)
            {
            }

            // Adds a binding; does nothing if already bound to this action
            void bind(A action, Binding binding);

            // Removes a binding; does nothing if not bound
            void unbind(A action, Binding binding);

            // Removes every binding of the action
            void clear(A action);

            // Bindings of the action, empty if none
            const std::vector<Binding>& bindings(A action) const noexcept;

            inline void setGamepad(int gamepad) noexcept { m_Gamepad = gamepad; }
            inline int gamepad() const noexcept { return m_Gamepad; }

            // True if any binding is held
            template <InputBackend I>
            bool isDown(A action, const I& input) const;

            // True if any binding was pressed this frame
            template <InputBackend I>
            bool isPressed(A action, const I& input) const;

            // True if any binding was released this frame
            template <InputBackend I>
            bool isReleased(A action, const I& input) const;
    };
}
