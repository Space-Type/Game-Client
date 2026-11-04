/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** menu controls
*/

#pragma once

#include <rtype/render.hpp>
#include <cstddef>


namespace RType::UI {
    enum class MenuAction {
        Up,
        Down,
        Left,
        Right,
        Confirm,
        Back,
        Count,  // Not an action: number of actions
    };

    inline constexpr std::size_t MENU_ACTION_COUNT = static_cast<std::size_t>(MenuAction::Count);

    using MenuControls = Render::ActionMap<MenuAction>;

    // Arrows, Tab, Enter + Escape on the keyboard, D-pad + A + B on a gamepad
    MenuControls defaultMenuControls();
}
