/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** player controls
*/

#include <gtest/gtest.h>
#include "../../src/game/Controls/Controls.hpp"
#include <algorithm>
#include <optional>

namespace {
    using RType::Game::Action;
    using RType::Game::Slot;
    using RType::Render::AxisBinding;
    using RType::Render::Binding;
    using RType::Render::GamepadAxis;
    using RType::Render::GamepadButton;
    using RType::Render::Key;
    using RType::Render::MouseButton;

    bool isBound(const RType::Game::Controls& controls, Action action, Binding binding)
    {
        return std::ranges::find(controls.bindings(action), binding) != controls.bindings(action).end();
    }
}

TEST(game_Controls, defaultSlots)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Up, Slot::Keyboard), Binding(Key::Up));
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Up, Slot::Gamepad), Binding(GamepadButton::DpadUp));
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Shoot, Slot::Keyboard), Binding(Key::Space));
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Shoot, Slot::Gamepad), Binding(GamepadButton::A));
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Force, Slot::Keyboard), Binding(Key::F));
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Force, Slot::Gamepad), Binding(GamepadButton::B));
    EXPECT_FALSE(RType::Game::conflicts(controls, Action::Force));
}

TEST(game_Controls, emptySlot)
{
    RType::Game::Controls controls;

    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Up, Slot::Keyboard), std::nullopt);
    controls.bind(Action::Up, AxisBinding{ GamepadAxis::LeftY, -0.5F });
    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Up, Slot::Gamepad), std::nullopt);
}

TEST(game_Controls, rebindReplacesOnlyItsSlot)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    RType::Game::rebind(controls, Action::Up, Slot::Keyboard, Key::W);

    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Up, Slot::Keyboard), Binding(Key::W));
    EXPECT_FALSE(isBound(controls, Action::Up, Key::Up));
    EXPECT_TRUE(isBound(controls, Action::Up, GamepadButton::DpadUp));
    EXPECT_TRUE(isBound(controls, Action::Up, AxisBinding{ GamepadAxis::LeftY, -0.5F }));
}

TEST(game_Controls, rebindAcceptsAMouseButtonInTheKeyboardSlot)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    RType::Game::rebind(controls, Action::Shoot, Slot::Keyboard, MouseButton::Left);

    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Shoot, Slot::Keyboard), Binding(MouseButton::Left));
    EXPECT_FALSE(isBound(controls, Action::Shoot, Key::Space));
}

TEST(game_Controls, rebindIgnoresABindingThatDoesNotFit)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    RType::Game::rebind(controls, Action::Shoot, Slot::Gamepad, Key::F);
    RType::Game::rebind(controls, Action::Shoot, Slot::Keyboard, GamepadButton::X);

    EXPECT_EQ(controls.bindings(Action::Shoot), RType::Game::defaultControls().bindings(Action::Shoot));
}

TEST(game_Controls, rebindFillsAnEmptySlot)
{
    RType::Game::Controls controls;

    RType::Game::rebind(controls, Action::Shoot, Slot::Gamepad, GamepadButton::X);

    EXPECT_EQ(RType::Game::slotBinding(controls, Action::Shoot, Slot::Gamepad), Binding(GamepadButton::X));
}

TEST(game_Controls, noConflictByDefault)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    for (Action action : RType::Game::ACTIONS)
        EXPECT_FALSE(RType::Game::conflicts(controls, action)) << RType::Game::actionName(action);
}

TEST(game_Controls, sharedBindingIsAConflictForBothActions)
{
    RType::Game::Controls controls = RType::Game::defaultControls();

    RType::Game::rebind(controls, Action::Left, Slot::Keyboard, Key::Space);

    EXPECT_TRUE(RType::Game::conflicts(controls, Action::Left));
    EXPECT_TRUE(RType::Game::conflicts(controls, Action::Shoot));
    EXPECT_FALSE(RType::Game::conflicts(controls, Action::Up));
}

TEST(game_Controls, sharedAxisIsNotAConflict)
{
    RType::Game::Controls controls;

    controls.bind(Action::Up, AxisBinding{ GamepadAxis::LeftY, -0.5F });
    controls.bind(Action::Shoot, AxisBinding{ GamepadAxis::LeftY, -0.5F });

    EXPECT_FALSE(RType::Game::conflicts(controls, Action::Up));
}

TEST(game_Controls, names)
{
    EXPECT_EQ(RType::Game::actionName(Action::Shoot), "Shoot");
    EXPECT_EQ(RType::Game::bindingName(Key::Space), "Space");
    EXPECT_EQ(RType::Game::bindingName(MouseButton::Right), "Mouse Right");
    EXPECT_EQ(RType::Game::bindingName(GamepadButton::DpadLeft), "D-pad Left");
}
