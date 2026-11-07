/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** in-game HUD
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>

using RType::UI::Hud;

TEST(ui_Hud, heartColourFollowsTheLives)
{
    using namespace RType::UI::Theme;

    EXPECT_EQ(Hud::heartColor(5), HEART_HIGH);
    EXPECT_EQ(Hud::heartColor(3), HEART_HIGH);
    EXPECT_EQ(Hud::heartColor(2), HEART_MEDIUM);
    EXPECT_EQ(Hud::heartColor(1), HEART_LOW);
}

TEST(ui_Hud, onlyTheLastLifeBlinks)
{
    for (float t : { 0.0F, 0.2F, 0.4F, 0.45F }) {
        EXPECT_TRUE(Hud::heartsVisible(3, t));
        EXPECT_TRUE(Hud::heartsVisible(2, t));
    }
    EXPECT_TRUE(Hud::heartsVisible(1, 0.1F));
    EXPECT_FALSE(Hud::heartsVisible(1, 0.45F));
    EXPECT_TRUE(Hud::heartsVisible(1, 0.6F));  // next period
}

TEST(ui_Hud, scoreHasSixDigits)
{
    EXPECT_EQ(Hud::scoreText(0), "000000");
    EXPECT_EQ(Hud::scoreText(1200), "001200");
    EXPECT_EQ(Hud::scoreText(4000000000U), "4000000000");
}

TEST(ui_Hud, announcementFadesInAndOut)
{
    EXPECT_EQ(Hud::announcementOpacity(0.0F, 2.5F), 0.0F);
    EXPECT_NEAR(Hud::announcementOpacity(Hud::FADE_IN / 2.0F, 2.5F), 0.5F, 0.001F);
    EXPECT_EQ(Hud::announcementOpacity(1.0F, 2.5F), 1.0F);
    EXPECT_NEAR(Hud::announcementOpacity(2.5F - Hud::FADE_OUT / 2.0F, 2.5F), 0.5F, 0.001F);
    EXPECT_EQ(Hud::announcementOpacity(3.0F, 2.5F), 0.0F);
    EXPECT_EQ(Hud::announcementOpacity(-1.0F, 2.5F), 0.0F);
}

TEST(ui_Hud, bossBarFollowsTheHealth)
{
    EXPECT_EQ(Hud::bossBarFill(60, 60, 600.0F), 600.0F);
    EXPECT_EQ(Hud::bossBarFill(30, 60, 600.0F), 300.0F);
    EXPECT_EQ(Hud::bossBarFill(0, 60, 600.0F), 0.0F);
    EXPECT_EQ(Hud::bossBarFill(90, 60, 600.0F), 600.0F);  // never past the full bar
    EXPECT_EQ(Hud::bossBarFill(10, 0, 600.0F), 0.0F);
}
