/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** starfield entities and their scrolling
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>
#include <cstddef>
#include <map>

namespace {
    using namespace std::chrono_literals;
    using RType::Components::Star;

    constexpr RType::Render::Vec2D AREA = { .x = 1280.0F, .y = 720.0F };

    struct Field {
        RType::Game::Engine engine;

        explicit Field(std::uint32_t seed = 1)
        {
            RType::Game::spawnStarfield(engine, AREA, seed);
            engine.get<RType::Systems::ScrollStars>().setArea(AREA);
        }

        // Stars by entity: the same entities for the same spawn order
        std::map<Wrecks::ECS::Entity, Star> stars() const
        {
            std::map<Wrecks::ECS::Entity, Star> all;

            for (auto [entity, star] : engine.get<Star>())
                all.emplace(entity, star);
            return all;
        }
    };
}

TEST(game_Starfield, starsAreInsideTheArea)
{
    Field field;

    ASSERT_FALSE(field.stars().empty());
    for (const auto& [entity, star] : field.stars()) {
        EXPECT_GE(star.position.x, 0.0F);
        EXPECT_LT(star.position.x, AREA.x);
        EXPECT_GE(star.position.y, 0.0F);
        EXPECT_LT(star.position.y, AREA.y);
    }
}

TEST(game_Starfield, sameSeedSameStars)
{
    Field a(7);
    Field b(7);
    auto starsA = a.stars();
    auto starsB = b.stars();

    ASSERT_EQ(starsA.size(), starsB.size());
    for (const auto& [entity, star] : starsA)
        EXPECT_EQ(star.position.x, starsB.at(entity).position.x);
}

TEST(game_Starfield, moreStarsFarAway)
{
    Field field;
    std::size_t farStars = 0;
    std::size_t nearStars = 0;

    for (const auto& [entity, star] : field.stars()) {
        farStars += star.size == 1.0F;
        nearStars += star.size == 3.0F;
    }
    EXPECT_EQ(field.stars().size(), 120);
    EXPECT_GT(farStars, nearStars);
}

TEST(game_Starfield, starsMoveLeftAtTheirSpeed)
{
    Field field;
    auto before = field.stars();

    field.engine.update(100ms);
    for (const auto& [entity, star] : field.stars()) {
        float expected = before.at(entity).position.x - star.speed * 0.1F;

        if (expected < 0.0F)
            expected += AREA.x;
        else
            EXPECT_EQ(star.position.y, before.at(entity).position.y);  // only a wrapping star changes height
        EXPECT_NEAR(star.position.x, expected, 0.01F);
    }
}

TEST(game_Starfield, independentOfTheFrameRate)
{
    Field oneStep;
    Field manySteps;

    oneStep.engine.update(1s);
    for (int i = 0; i < 60; ++i)
        manySteps.engine.update(std::chrono::duration<double, std::milli>(1000.0 / 60.0));

    auto one = oneStep.stars();

    for (const auto& [entity, star] : manySteps.stars())
        EXPECT_NEAR(star.position.x, one.at(entity).position.x, 0.05F);
}

TEST(game_Starfield, starsWrapAroundAfterALongTime)
{
    Field field;

    field.engine.update(1h);
    for (const auto& [entity, star] : field.stars()) {
        EXPECT_GE(star.position.x, 0.0F);
        EXPECT_LT(star.position.x, AREA.x);
    }
}

TEST(game_Starfield, wrappingStarsTakeANewHeight)
{
    Field field;
    auto before = field.stars();

    field.engine.update(2min);  // every star wraps at least once
    for (const auto& [entity, star] : field.stars()) {
        EXPECT_GE(star.position.y, 0.0F);
        EXPECT_LT(star.position.y, AREA.y);
        EXPECT_NE(star.position.y, before.at(entity).position.y);
    }
}

TEST(game_Starfield, scrollWrapsInsideTheWidth)
{
    Star star{ .position = { .x = 10.0F, .y = 5.0F }, .size = 1.0F, .speed = 100.0F, .color = {} };

    EXPECT_TRUE(RType::Components::scroll(star, 0.5F, 100.0F));  // 10 - 50 = -40 -> 60
    EXPECT_NEAR(star.position.x, 60.0F, 0.001F);
    EXPECT_FALSE(RType::Components::scroll(star, 0.1F, 100.0F));
    EXPECT_NEAR(star.position.x, 50.0F, 0.001F);
}
