/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** PlayerInput system
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>

using namespace std::chrono_literals;

// Disabled (a menu is open), the system clears the input of the ships it handles: only this client's ship
TEST(systems_PlayerInput, onlyTheLocalShipTakesTheLocalInput)
{
    RType::Game::Engine engine;
    auto local = engine.createEntity();
    auto other = engine.createEntity();

    engine.addComponent(local, Protecs::Components::PlayerInput{ .up = true, .down = false, .left = false, .right = false, .trigger = true });
    engine.addComponent(local, RType::Components::Controllable{});
    engine.addComponent(other, Protecs::Components::PlayerInput{ .up = true, .down = false, .left = false, .right = false, .trigger = true });
    engine.get<RType::Systems::PlayerInput>().setEnabled(false);

    engine.update(10ms);
    EXPECT_FALSE(engine.get<Protecs::Components::PlayerInput>()[local].up);
    EXPECT_FALSE(engine.get<Protecs::Components::PlayerInput>()[local].trigger);
    EXPECT_TRUE(engine.get<Protecs::Components::PlayerInput>()[other].up);  // what the server shared
    EXPECT_TRUE(engine.get<Protecs::Components::PlayerInput>()[other].trigger);
}
