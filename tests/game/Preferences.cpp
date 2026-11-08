/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** player preferences
*/

#include <gtest/gtest.h>
#include "../../src/game/Preferences/Preferences.hpp"
#include <cstddef>
#include <optional>
#include <string>

namespace {
    using RType::Game::Action;
    using RType::Game::Preferences;
    using RType::Game::Slot;
    using RType::Render::Binding;
    using RType::Render::GamepadButton;
    using RType::Render::Key;
    using RType::Render::MouseButton;

    // The input with the longest name among those that fit the slot
    Binding longestBinding(Slot slot)
    {
        Binding longest = slot == Slot::Keyboard ? Binding(Key::A) : Binding(GamepadButton::A);
        auto consider = [&](Binding binding) {
            if (RType::Game::bindingName(binding).size() > RType::Game::bindingName(longest).size())
                longest = binding;
        };

        if (slot == Slot::Keyboard) {
            for (std::size_t i = 0; i < static_cast<std::size_t>(Key::Count); ++i)
                consider(static_cast<Key>(i));
            for (std::size_t i = 0; i < static_cast<std::size_t>(MouseButton::Count); ++i)
                consider(static_cast<MouseButton>(i));
        } else {
            for (std::size_t i = 0; i < static_cast<std::size_t>(GamepadButton::Count); ++i)
                consider(static_cast<GamepadButton>(i));
        }
        return longest;
    }

    std::optional<Binding> slot(const Preferences& preferences, Action action, Slot slot)
    {
        return RType::Game::slotBinding(preferences.controls, action, slot);
    }
}

TEST(game_Preferences, defaultsText)
{
    std::string text = RType::Game::toText(Preferences{});

    EXPECT_NE(text.find("volume.master=100\n"), std::string::npos);
    EXPECT_NE(text.find("fullscreen=false\n"), std::string::npos);
    EXPECT_NE(text.find("accessibility.high_contrast=false\n"), std::string::npos);
    EXPECT_NE(text.find("bind.up.gamepad=D-pad Up\n"), std::string::npos);
    EXPECT_NE(text.find("bind.shoot.keyboard=Space\n"), std::string::npos);
}

TEST(game_Preferences, roundTrip)
{
    Preferences preferences{ .master = 80, .music = 0, .effects = 35, .fullscreen = true, .highContrast = true };

    RType::Game::rebind(preferences.controls, Action::Shoot, Slot::Keyboard, MouseButton::Left);
    RType::Game::rebind(preferences.controls, Action::Up, Slot::Keyboard, Key::LeftShift);
    RType::Game::rebind(preferences.controls, Action::Left, Slot::Gamepad, GamepadButton::X);

    Preferences read = RType::Game::fromText(RType::Game::toText(preferences), Preferences{});

    EXPECT_EQ(read.master, 80);
    EXPECT_EQ(read.music, 0);
    EXPECT_EQ(read.effects, 35);
    EXPECT_TRUE(read.fullscreen);
    EXPECT_TRUE(read.highContrast);
    for (Action action : RType::Game::ACTIONS)
        for (Slot s : { Slot::Keyboard, Slot::Gamepad })
            EXPECT_EQ(slot(read, action, s), slot(preferences, action, s)) << RType::Game::actionName(action);
}

TEST(game_Preferences, chosenGamepadRoundTrip)
{
    Preferences chosen{ .gamepad = "Xbox Wireless Controller" };

    EXPECT_EQ(RType::Game::fromText(RType::Game::toText(chosen), Preferences{}).gamepad, "Xbox Wireless Controller");
    EXPECT_EQ(RType::Game::fromText(RType::Game::toText(Preferences{}), Preferences{ .gamepad = "Old" }).gamepad, "");
}

TEST(game_Preferences, emptySlotRoundTrip)
{
    Preferences preferences;

    preferences.controls.unbind(Action::Shoot, GamepadButton::A);

    Preferences read = RType::Game::fromText(RType::Game::toText(preferences), Preferences{});

    EXPECT_EQ(slot(read, Action::Shoot, Slot::Gamepad), std::nullopt);
}

TEST(game_Preferences, axesAreKept)
{
    Preferences read = RType::Game::fromText("bind.up.gamepad=Y\n", Preferences{});

    EXPECT_EQ(read.controls.bindings(Action::Up).size(), Preferences{}.controls.bindings(Action::Up).size());
}

TEST(game_Preferences, badLinesKeepTheDefaults)
{
    Preferences read = RType::Game::fromText(
        "garbage\n"
        "=\n"
        "volume.master=500\n"
        "volume.music=-1\n"
        "volume.effects=50abc\n"
        "fullscreen=yes\n"
        "accessibility.high_contrast=1\n"
        "bind.shoot.keyboard=NotAKey\n"
        "bind.shoot.gamepad=Space\n"
        "bind.jump.keyboard=J\n"
        "unknown.key=1\n",
        Preferences{});

    EXPECT_EQ(read.master, 100);
    EXPECT_EQ(read.music, 100);
    EXPECT_EQ(read.effects, 100);
    EXPECT_FALSE(read.fullscreen);
    EXPECT_FALSE(read.highContrast);
    EXPECT_EQ(read.controls.bindings(Action::Shoot), Preferences{}.controls.bindings(Action::Shoot));
}

TEST(game_Preferences, spacesAndWindowsLineEndings)
{
    Preferences read = RType::Game::fromText("volume.music = 40\r\nbind.down.keyboard = Left Shift\r\n", Preferences{});

    EXPECT_EQ(read.music, 40);
    EXPECT_EQ(slot(read, Action::Down, Slot::Keyboard), Binding(Key::LeftShift));
}

TEST(game_Preferences, emptyText)
{
    Preferences read = RType::Game::fromText("", Preferences{ .master = 10 });

    EXPECT_EQ(read.master, 10);
}

// If this fails, a file the game writes would be ignored at the next start: raise FILE_SIZE_MAX
TEST(game_Preferences, biggestFileFitsTheLimit)
{
    Preferences preferences;

    for (Action action : RType::Game::ACTIONS)
        for (Slot s : { Slot::Keyboard, Slot::Gamepad })
            RType::Game::rebind(preferences.controls, action, s, longestBinding(s));

    EXPECT_LE(RType::Game::toText(preferences).size(), Preferences::FILE_SIZE_MAX);
}
