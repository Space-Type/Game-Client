/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** player controls
*/

#pragma once

#include <rtype/render.hpp>


namespace RType::Game {
    enum class Action {
        Up,
        Down,
        Left,
        Right,
        Shoot,
    };

    // Remappable at runtime (accessibility): bind / unbind / clear
    using Controls = Render::ActionMap<Action>;

    // Arrows + Space on the keyboard, D-pad / left stick + A on a gamepad
    Controls defaultControls();
}
