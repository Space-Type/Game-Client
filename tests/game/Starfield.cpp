/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Starfield class
*/

#include <gtest/gtest.h>
#include "../../src/game/Starfield/Starfield.hpp"
#include <chrono>
#include <cstddef>
#include <vector>

namespace {
    using namespace std::chrono_literals;
    using RType::Game::Starfield;

    constexpr RType::Render::Vec2D SIZE = { .x = 1280.0F, .y = 720.0F };
}

TEST(game_Starfield, starsAreInsideTheArea)
{
    Starfield field(SIZE);

    ASSERT_FALSE(field.stars().empty());
    for (const Starfield::Star& star : field.stars()) {
        EXPECT_GE(star.position.x, 0.0F);
        EXPECT_LT(star.position.x, SIZE.x);
        EXPECT_GE(star.position.y, 0.0F);
        EXPECT_LT(star.position.y, SIZE.y);
    }
}

TEST(game_Starfield, sameSeedSameStars)
{
    Starfield a(SIZE, 7);
    Starfield b(SIZE, 7);

    for (std::size_t i = 0; i < a.stars().size(); ++i)
        EXPECT_EQ(a.stars()[i].position.x, b.stars()[i].position.x);
}

TEST(game_Starfield, starsMoveLeftAtTheirSpeed)
{
    Starfield field(SIZE);
    Starfield moved(SIZE);

    moved.update(100ms);
    for (std::size_t i = 0; i < field.stars().size(); ++i) {
        float expected = field.stars()[i].position.x - field.stars()[i].speed * 0.1F;

        if (expected < 0.0F)
            expected += SIZE.x;
        else
            EXPECT_EQ(moved.stars()[i].position.y, field.stars()[i].position.y);  // only a wrapping star changes height
        EXPECT_NEAR(moved.stars()[i].position.x, expected, 0.01F);
    }
}

TEST(game_Starfield, independentOfTheFrameRate)
{
    Starfield oneStep(SIZE);
    Starfield manySteps(SIZE);

    oneStep.update(1s);
    for (int i = 0; i < 60; ++i)
        manySteps.update(std::chrono::duration<double, std::milli>(1000.0 / 60.0));
    for (std::size_t i = 0; i < oneStep.stars().size(); ++i)
        EXPECT_NEAR(oneStep.stars()[i].position.x, manySteps.stars()[i].position.x, 0.05F);
}

TEST(game_Starfield, starsWrapAroundAfterALongTime)
{
    Starfield field(SIZE);

    field.update(1h);
    for (const Starfield::Star& star : field.stars()) {
        EXPECT_GE(star.position.x, 0.0F);
        EXPECT_LT(star.position.x, SIZE.x);
    }
}

TEST(game_Starfield, moreStarsFarAway)
{
    Starfield field(SIZE);
    std::size_t far = 0;
    std::size_t near = 0;

    for (const Starfield::Star& star : field.stars()) {
        far += star.size == 1.0F;
        near += star.size == 3.0F;
    }
    EXPECT_EQ(field.stars().size(), 120);
    EXPECT_GT(far, near);
}

TEST(game_Starfield, wrappingStarsTakeANewHeight)
{
    Starfield field(SIZE);
    std::vector<float> before;

    for (const Starfield::Star& star : field.stars())
        before.push_back(star.position.y);
    field.update(std::chrono::minutes(2));  // every star wrapped at least once

    std::size_t changed = 0;

    for (std::size_t i = 0; i < before.size(); ++i) {
        EXPECT_GE(field.stars()[i].position.y, 0.0F);
        EXPECT_LT(field.stars()[i].position.y, SIZE.y);
        changed += field.stars()[i].position.y != before[i];
    }
    EXPECT_EQ(changed, before.size());
}
