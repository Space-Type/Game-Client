/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** ForgetStale system
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>

namespace {
    using namespace std::chrono_literals;
    using RType::Components::Freshness;
    using RType::Components::STALE_AFTER;

    struct World {
        RType::Game::Engine engine;
        RType::Utils::EntityMap map;
        std::chrono::steady_clock::time_point lastComponentAt = std::chrono::steady_clock::now();

        World()
        {
            engine.get<RType::Systems::ForgetStale>().setEntityMap(map);
            engine.get<RType::Systems::ForgetStale>().setLastComponentAt(lastComponentAt);
        }

        Wrecks::ECS::Entity networked(Protecs::NetworkEntity id, float sinceUpdate)
        {
            auto entity = engine.createEntity();

            engine.addComponent(entity, Freshness{ .sinceUpdate = sinceUpdate });
            map.bind(id, entity);
            return entity;
        }
    };
}

TEST(systems_ForgetStale, entityTheServerStoppedUpdatingIsForgotten)
{
    World world;
    auto stale = world.networked(10, STALE_AFTER + 0.1F);
    auto fresh = world.networked(11, 0.0F);

    world.engine.update(10ms);
    EXPECT_FALSE(world.engine.hasComponent<Freshness>(stale));
    EXPECT_FALSE(world.map.containsNetwork(Protecs::NetworkEntity(10)));
    EXPECT_TRUE(world.engine.hasComponent<Freshness>(fresh));
    EXPECT_TRUE(world.map.containsNetwork(Protecs::NetworkEntity(11)));
}

TEST(systems_ForgetStale, freshnessGrowsWithTime)
{
    World world;
    auto entity = world.networked(10, 0.0F);

    world.engine.update(200ms);
    EXPECT_NEAR(world.engine.get<Freshness>()[entity].sinceUpdate, 0.2F, 0.001F);
}

TEST(systems_ForgetStale, localShipIsNeverForgotten)
{
    World world;
    auto local = world.networked(10, STALE_AFTER + 5.0F);

    world.engine.addComponent(local, RType::Components::LocalCharge{});
    world.engine.update(10ms);
    EXPECT_TRUE(world.engine.hasComponent<Freshness>(local));
}

TEST(systems_ForgetStale, nothingIsForgottenWhenNothingArrives)
{
    World world;
    auto entity = world.networked(10, STALE_AFTER + 5.0F);

    world.lastComponentAt = std::chrono::steady_clock::now() - 10s;  // server or network hiccup
    world.engine.update(10ms);
    EXPECT_TRUE(world.engine.hasComponent<Freshness>(entity));
}
