/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** stage background view
*/

#include <gtest/gtest.h>
#include <rtype/systems.hpp>

namespace {
    constexpr RType::Render::Vec2D STAGE_1 = { .x = 4224.0F, .y = 240.0F };
    constexpr RType::Render::Vec2D SCREEN = { .x = 1280.0F, .y = 720.0F };
}

TEST(systems_RenderStage, viewIsTheScreenOnTheBackground)
{
    RType::Render::Rect view = RType::Systems::stageView(0.0F, STAGE_1, SCREEN);

    EXPECT_EQ(view.x, 0.0F);
    EXPECT_EQ(view.height, 240.0F);
    EXPECT_NEAR(view.width, 426.67F, 0.01F);  // 1280 / 3: the background is scaled 3 times
}

TEST(systems_RenderStage, viewFollowsTheProgression)
{
    EXPECT_EQ(RType::Systems::stageView(1000.0F, STAGE_1, SCREEN).x, 1000.0F);
}

TEST(systems_RenderStage, viewStopsAtTheEnd)
{
    RType::Render::Rect view = RType::Systems::stageView(10000.0F, STAGE_1, SCREEN);

    EXPECT_NEAR(view.x + view.width, STAGE_1.x, 0.01F);
}

TEST(systems_RenderStage, emptyTextureGivesNothing)
{
    RType::Render::Rect view = RType::Systems::stageView(10.0F, { .x = 0.0F, .y = 0.0F }, SCREEN);

    EXPECT_EQ(view.width, 0.0F);
}
