/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ActionMap class
*/

#pragma once

#include "Concepts.hpp"
#include "InputTypes.hpp"
#include <algorithm>
#include <type_traits>
#include <unordered_map>
#include <variant>
#include <vector>

namespace RType::Render {

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

            // Bindings of the action, empty if none.
            // The reference is invalidated by bind / unbind / clear
            const std::vector<Binding>& bindings(A action) const noexcept;

            inline void setGamepad(int gamepad) noexcept { m_Gamepad = gamepad; }
            inline int gamepad() const noexcept { return m_Gamepad; }

            // True if any binding is held
            template <InputType I>
            bool isDown(A action, const I& input) const;

            // True if any binding was pressed this frame
            template <InputType I>
            bool isPressed(A action, const I& input) const;

            // True if any binding was released this frame
            template <InputType I>
            bool isReleased(A action, const I& input) const;
    };
}


/////////////////////////////////////////////////////////
//                                                     //
//                 TEMPLATE DEFINITIONS                //
//                                                     //
/////////////////////////////////////////////////////////

template <typename A>
requires (std::is_enum_v<A>)
void RType::Render::ActionMap<A>::bind(A action, Binding binding)
{
    std::vector<Binding>& list = m_Bindings[action];

    if (std::find(list.begin(), list.end(), binding) == list.end())
        list.push_back(binding);
}

template <typename A>
requires (std::is_enum_v<A>)
void RType::Render::ActionMap<A>::unbind(A action, Binding binding)
{
    auto it = m_Bindings.find(action);

    if (it == m_Bindings.end())
        return;
    std::erase(it->second, binding);
    if (it->second.empty())
        m_Bindings.erase(it);
}

template <typename A>
requires (std::is_enum_v<A>)
void RType::Render::ActionMap<A>::clear(A action)
{
    m_Bindings.erase(action);
}

template <typename A>
requires (std::is_enum_v<A>)
const std::vector<RType::Render::Binding>& RType::Render::ActionMap<A>::bindings(A action) const noexcept
{
    static const std::vector<Binding> s_Empty;
    auto it = m_Bindings.find(action);

    return it == m_Bindings.end() ? s_Empty : it->second;
}

template <typename A>
requires (std::is_enum_v<A>)
template <RType::Render::InputType I>
bool RType::Render::ActionMap<A>::isDown(A action, const I& input) const
{
    return std::ranges::any_of(bindings(action), [&](const Binding& binding) {
        return std::visit([&](const auto& value) -> bool {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, GamepadButton>) {
                return input.isDown(m_Gamepad, value);
            } else if constexpr (std::is_same_v<T, AxisBinding>) {
                if (value.threshold > 0.0F)
                    return input.axis(m_Gamepad, value.axis) >= value.threshold;
                if (value.threshold < 0.0F)
                    return input.axis(m_Gamepad, value.axis) <= value.threshold;
                return false;
            } else {
                return input.isDown(value);
            }
        }, binding);
    });
}

template <typename A>
requires (std::is_enum_v<A>)
template <RType::Render::InputType I>
bool RType::Render::ActionMap<A>::isPressed(A action, const I& input) const
{
    return std::ranges::any_of(bindings(action), [&](const Binding& binding) {
        return std::visit([&](const auto& value) -> bool {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, GamepadButton>)
                return input.isPressed(m_Gamepad, value);
            else if constexpr (std::is_same_v<T, AxisBinding>)
                return false;
            else
                return input.isPressed(value);
        }, binding);
    });
}

template <typename A>
requires (std::is_enum_v<A>)
template <RType::Render::InputType I>
bool RType::Render::ActionMap<A>::isReleased(A action, const I& input) const
{
    return std::ranges::any_of(bindings(action), [&](const Binding& binding) {
        return std::visit([&](const auto& value) -> bool {
            using T = std::decay_t<decltype(value)>;

            if constexpr (std::is_same_v<T, GamepadButton>)
                return input.isReleased(m_Gamepad, value);
            else if constexpr (std::is_same_v<T, AxisBinding>)
                return false;
            else
                return input.isReleased(value);
        }, binding);
    });
}
