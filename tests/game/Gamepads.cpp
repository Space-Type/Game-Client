/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** gamepad choice
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <vector>

namespace {
    using RType::Game::chooseGamepad;
    using RType::Game::ConnectedGamepad;
    using RType::Game::nextGamepadChoice;

    // Indexes 0 and 1 are taken by joysticks without a mapping (a keyboard with a joystick interface)
    const std::vector<ConnectedGamepad> TWO = {
        { .index = 2, .name = "Xbox Wireless Controller" },
        { .index = 3, .name = "PS5 Controller" },
    };
}

TEST(game_Gamepads, noGamepadGivesNone)
{
    EXPECT_EQ(chooseGamepad({}, ""), -1);
    EXPECT_EQ(chooseGamepad({}, "Xbox Wireless Controller"), -1);
}

TEST(game_Gamepads, firstGamepadFoundByDefault)
{
    EXPECT_EQ(chooseGamepad(TWO, ""), 2);
}

TEST(game_Gamepads, chosenGamepadByName)
{
    EXPECT_EQ(chooseGamepad(TWO, "PS5 Controller"), 3);
}

TEST(game_Gamepads, chosenGamepadUnpluggedFallsBackToTheFirst)
{
    EXPECT_EQ(chooseGamepad(TWO, "8BitDo Pro 2"), 2);
}

TEST(game_Gamepads, settingsCycleThroughFirstFoundThenEachGamepad)
{
    EXPECT_EQ(nextGamepadChoice(TWO, ""), "Xbox Wireless Controller");
    EXPECT_EQ(nextGamepadChoice(TWO, "Xbox Wireless Controller"), "PS5 Controller");
    EXPECT_EQ(nextGamepadChoice(TWO, "PS5 Controller"), "");
    EXPECT_EQ(nextGamepadChoice(TWO, "8BitDo Pro 2"), "Xbox Wireless Controller");
    EXPECT_EQ(nextGamepadChoice({}, "PS5 Controller"), "");
}
