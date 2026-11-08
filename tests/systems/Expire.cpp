/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Expire system
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>

namespace {
    using namespace std::chrono_literals;
    using RType::Components::Lifetime;
}

TEST(systems_Expire, entityIsDeletedWhenItsLifetimeRunsOut)
{
    RType::Game::Engine engine;
    auto effect = engine.createEntity();

    engine.addComponent(effect, Lifetime{ .remaining = 0.5F });
    engine.update(300ms);
    EXPECT_TRUE(engine.hasComponent<Lifetime>(effect));
    EXPECT_NEAR(engine.get<Lifetime>()[effect].remaining, 0.2F, 0.001F);

    engine.update(300ms);
    EXPECT_FALSE(engine.hasComponent<Lifetime>(effect));
}

TEST(systems_Expire, severalEntitiesExpireInTheSameUpdate)
{
    RType::Game::Engine engine;
    auto first = engine.createEntity();
    auto kept = engine.createEntity();
    auto second = engine.createEntity();

    engine.addComponent(first, Lifetime{ .remaining = 0.1F });
    engine.addComponent(kept, Lifetime{ .remaining = 5.0F });
    engine.addComponent(second, Lifetime{ .remaining = 0.1F });
    engine.update(200ms);

    EXPECT_FALSE(engine.hasComponent<Lifetime>(first));
    EXPECT_TRUE(engine.hasComponent<Lifetime>(kept));
    EXPECT_FALSE(engine.hasComponent<Lifetime>(second));
}

TEST(systems_Expire, entitiesWithoutLifetimeStay)
{
    RType::Game::Engine engine;
    auto entity = engine.createEntity();

    engine.addComponent(entity, RType::Components::Controllable{});
    engine.update(10s);
    EXPECT_TRUE(engine.hasComponent<RType::Components::Controllable>(entity));
}
