/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** sprite animation
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>
#include <rtype/systems.hpp>

namespace {
    using RType::Components::Sprite;

    Sprite animation(bool loop, float elapsed)
    {
        return Sprite{
            .frame = RType::Render::Rect{ .x = 10.0F, .y = 20.0F, .width = 33.0F, .height = 17.0F },
            .frameCount = 4,
            .framesPerSecond = 10.0F,
            .loop = loop,
            .elapsed = elapsed,
        };
    }
}

TEST(systems_RenderSprite, stillSpriteStaysOnTheFirstFrame)
{
    EXPECT_EQ(RType::Components::frameIndex(Sprite{ .elapsed = 5.0F }), 0);
    EXPECT_EQ(RType::Components::frameAt(Sprite{}, 0), std::nullopt);
}

TEST(systems_RenderSprite, loopingAnimation)
{
    EXPECT_EQ(RType::Components::frameIndex(animation(true, 0.0F)), 0);
    EXPECT_EQ(RType::Components::frameIndex(animation(true, 0.25F)), 2);
    EXPECT_EQ(RType::Components::frameIndex(animation(true, 0.45F)), 0);  // 4th frame played, back to the first
    EXPECT_FALSE(RType::Components::finished(animation(true, 100.0F)));
}

TEST(systems_RenderSprite, animationPlayedOnce)
{
    EXPECT_EQ(RType::Components::frameIndex(animation(false, 0.35F)), 3);
    EXPECT_EQ(RType::Components::frameIndex(animation(false, 2.0F)), 3);
    EXPECT_FALSE(RType::Components::finished(animation(false, 0.35F)));
    EXPECT_TRUE(RType::Components::finished(animation(false, 0.4F)));
}

TEST(systems_RenderSprite, framesAreSideBySide)
{
    auto frame = RType::Components::frameAt(animation(true, 0.0F), 2);

    ASSERT_TRUE(frame);
    EXPECT_EQ(frame->x, 76.0F);
    EXPECT_EQ(frame->y, 20.0F);
    EXPECT_EQ(frame->width, 33.0F);
}

TEST(systems_RenderSprite, shipTiltsWithItsVelocity)
{
    using RType::Systems::RenderSprite;

    EXPECT_EQ(RType::Systems::tiltFrame(0.0F), RenderSprite::STILL_FRAME);
    EXPECT_EQ(RType::Systems::tiltFrame(-3.0F), RenderSprite::TILT_UP_FRAME);
    EXPECT_EQ(RType::Systems::tiltFrame(3.0F), RenderSprite::TILT_DOWN_FRAME);
}
