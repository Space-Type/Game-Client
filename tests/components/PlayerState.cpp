/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** player state shown by the client
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

TEST(components_PlayerState, blinkAlternates)
{
    using RType::Components::BLINK_PERIOD;
    using RType::Components::blinkVisible;

    EXPECT_TRUE(blinkVisible(BLINK_PERIOD * 3.0F + 0.01F));   // first half of a period
    EXPECT_FALSE(blinkVisible(BLINK_PERIOD * 3.0F + 0.15F));  // second half
}

TEST(components_PlayerState, visibleOnceTheBlinkIsOver)
{
    EXPECT_TRUE(RType::Components::blinkVisible(0.0F));
    EXPECT_TRUE(RType::Components::blinkVisible(-1.0F));
}

TEST(components_PlayerState, blinkLastsAsLongAsTheInvulnerability)
{
    EXPECT_EQ(RType::Components::Blink{}.remaining, RType::Components::BLINK_DURATION);
    EXPECT_EQ(RType::Components::BLINK_DURATION, 2.0F);
}
