/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Announcement component
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>

namespace {
    using namespace std::chrono_literals;
    using RType::Components::Announcement;
    using RType::Components::Lifetime;
}

TEST(components_Announcement, shownForIsTheTimeSinceItStarted)
{
    Announcement announcement{ .text = "LEVEL 1", .duration = 2.5F, .scores = false };

    EXPECT_FLOAT_EQ(RType::Components::shownFor(announcement, Lifetime{ .remaining = 2.5F }), 0.0F);
    EXPECT_FLOAT_EQ(RType::Components::shownFor(announcement, Lifetime{ .remaining = 1.0F }), 1.5F);
}

TEST(components_Announcement, goesAwayAfterItsDuration)
{
    RType::Game::Engine engine;
    auto entity = engine.createEntity();

    engine.addComponent(entity, Announcement{ .text = "STAGE CLEAR", .duration = 1.0F, .scores = true });
    engine.addComponent(entity, Lifetime{ .remaining = 1.0F });
    engine.update(600ms);
    EXPECT_TRUE(engine.hasComponent<Announcement>(entity));
    engine.update(600ms);
    EXPECT_FALSE(engine.hasComponent<Announcement>(entity));
}
