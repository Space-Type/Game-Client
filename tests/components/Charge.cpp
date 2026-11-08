/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** charged shot state shown by the client
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

using RType::Components::CHARGE_LEVEL_TIME;
using RType::Components::chargeLevel;

TEST(components_Charge, noChargeBeforeTheFirstLevel)
{
    EXPECT_EQ(chargeLevel(0.0F), 0);
    EXPECT_EQ(chargeLevel(CHARGE_LEVEL_TIME * 0.9F), 0);
}

TEST(components_Charge, oneLevelPerStep)
{
    EXPECT_EQ(chargeLevel(CHARGE_LEVEL_TIME * 1.1F), 1);
    EXPECT_EQ(chargeLevel(CHARGE_LEVEL_TIME * 3.5F), 3);
}

TEST(components_Charge, fullAfterTwoSeconds)
{
    EXPECT_EQ(chargeLevel(2.0F), RType::Components::CHARGE_LEVELS);
    EXPECT_EQ(chargeLevel(60.0F), RType::Components::CHARGE_LEVELS);
    EXPECT_EQ(chargeLevel(-1.0F), 0);
}

TEST(components_Charge, quickTapIsSilent)
{
    EXPECT_LT(RType::Components::CHARGE_SOUND_DELAY, CHARGE_LEVEL_TIME);
}
