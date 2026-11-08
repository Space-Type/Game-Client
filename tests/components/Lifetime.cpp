/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Lifetime component
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

using RType::Components::animationDuration;
using RType::Components::Sprite;

TEST(components_Lifetime, animationPlayedOnceLastsAllItsFrames)
{
    Sprite sprite{ .frameCount = 6, .framesPerSecond = 12.0F, .loop = false };

    EXPECT_FLOAT_EQ(animationDuration(sprite), 0.5F);
}

TEST(components_Lifetime, loopingOrStillSpriteHasNoEnd)
{
    EXPECT_EQ(animationDuration(Sprite{ .frameCount = 6, .framesPerSecond = 12.0F, .loop = true }), 0.0F);
    EXPECT_EQ(animationDuration(Sprite{ .frameCount = 1, .framesPerSecond = 0.0F, .loop = false }), 0.0F);
}

TEST(components_Lifetime, endsWhenTheAnimationIsFinished)
{
    Sprite sprite{ .frameCount = 6, .framesPerSecond = 12.0F, .loop = false };

    sprite.elapsed = animationDuration(sprite);
    EXPECT_TRUE(RType::Components::finished(sprite));
}
