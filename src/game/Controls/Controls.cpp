/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** player controls
*/

#include "Controls.hpp"


auto RType::Game::defaultControls() -> Controls
{
    using Render::AxisBinding;
    using Render::GamepadAxis;
    using Render::GamepadButton;
    using Render::Key;

    Controls controls;

    controls.bind(Action::Up, Key::Up);
    controls.bind(Action::Up, GamepadButton::DpadUp);
    controls.bind(Action::Up, AxisBinding{ GamepadAxis::LeftY, -0.5f });
    controls.bind(Action::Down, Key::Down);
    controls.bind(Action::Down, GamepadButton::DpadDown);
    controls.bind(Action::Down, AxisBinding{ GamepadAxis::LeftY, 0.5f });
    controls.bind(Action::Left, Key::Left);
    controls.bind(Action::Left, GamepadButton::DpadLeft);
    controls.bind(Action::Left, AxisBinding{ GamepadAxis::LeftX, -0.5f });
    controls.bind(Action::Right, Key::Right);
    controls.bind(Action::Right, GamepadButton::DpadRight);
    controls.bind(Action::Right, AxisBinding{ GamepadAxis::LeftX, 0.5f });
    controls.bind(Action::Shoot, Key::Space);
    controls.bind(Action::Shoot, GamepadButton::A);

    return controls;
}
