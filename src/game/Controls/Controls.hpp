/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** player controls
*/

#pragma once

#include <rtype/render.hpp>
#include <array>
#include <optional>
#include <string_view>


namespace RType::Game {
    enum class Action {
        Up,
        Down,
        Left,
        Right,
        Shoot,
    };

    inline constexpr std::array ACTIONS = { Action::Up, Action::Down, Action::Left, Action::Right, Action::Shoot };

    // Remappable at runtime (accessibility): bind / unbind / clear
    using Controls = Render::ActionMap<Action>;

    /**
     * The bindings a player can change: one key or mouse
     * button, and one gamepad button per action. Axis
     * bindings (left stick) are never part of a slot.
     */
    enum class Slot {
        Keyboard,
        Gamepad,
    };

    // Arrows + Space on the keyboard, D-pad / left stick + A on a gamepad
    Controls defaultControls();

    // "Up", ..., "Shoot"
    std::string_view actionName(Action action) noexcept;

    // Name of a key, mouse or gamepad button ("Space", "Mouse Left", "A"), "Axis" for an axis
    std::string_view bindingName(const Render::Binding& binding) noexcept;

    // Whether the binding can go in the slot (keys and mouse buttons in Keyboard, gamepad buttons in Gamepad)
    bool fitsSlot(const Render::Binding& binding, Slot slot) noexcept;

    // First binding of the action that fits the slot, nullopt if none
    std::optional<Render::Binding> slotBinding(const Controls& controls, Action action, Slot slot);

    // Replaces the slot's binding (adds one if the slot was empty); does nothing if the binding does not fit
    void rebind(Controls& controls, Action action, Slot slot, Render::Binding binding);

    // True if another action shares one of the action's slot bindings
    bool conflicts(const Controls& controls, Action action);
}
