/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** ChargeSound component
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

using RType::Components::CHARGE_SOUND_DELAY;
using RType::Components::chargeSoundChange;
using RType::Components::SoundChange;

TEST(components_ChargeSound, quickTapPlaysNothing)
{
    EXPECT_EQ(chargeSoundChange(0.0F, false), SoundChange::None);
    EXPECT_EQ(chargeSoundChange(CHARGE_SOUND_DELAY / 2.0F, false), SoundChange::None);
}

TEST(components_ChargeSound, holdingTheTriggerPlaysItOnce)
{
    EXPECT_EQ(chargeSoundChange(CHARGE_SOUND_DELAY, false), SoundChange::Play);
    EXPECT_EQ(chargeSoundChange(CHARGE_SOUND_DELAY + 1.0F, true), SoundChange::None);
}

TEST(components_ChargeSound, releasingStopsIt)
{
    EXPECT_EQ(chargeSoundChange(0.0F, true), SoundChange::Stop);
}
