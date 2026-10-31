/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** menu controls
*/

#include "MenuControls.hpp"


auto RType::UI::defaultMenuControls() -> MenuControls
{
    using Render::GamepadButton;
    using Render::Key;

    MenuControls controls;

    controls.bind(MenuAction::Up, Key::Up);
    controls.bind(MenuAction::Up, GamepadButton::DpadUp);
    controls.bind(MenuAction::Down, Key::Down);
    controls.bind(MenuAction::Down, Key::Tab);
    controls.bind(MenuAction::Down, GamepadButton::DpadDown);
    controls.bind(MenuAction::Left, Key::Left);
    controls.bind(MenuAction::Left, GamepadButton::DpadLeft);
    controls.bind(MenuAction::Right, Key::Right);
    controls.bind(MenuAction::Right, GamepadButton::DpadRight);
    controls.bind(MenuAction::Confirm, Key::Enter);
    controls.bind(MenuAction::Confirm, Key::KpEnter);
    controls.bind(MenuAction::Confirm, GamepadButton::A);
    controls.bind(MenuAction::Back, Key::Escape);
    controls.bind(MenuAction::Back, GamepadButton::B);

    return controls;
}
