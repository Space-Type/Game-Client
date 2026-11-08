/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** the Popup component
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

using namespace RType::Components;

TEST(components_Popup, staysOpaqueThenFades)
{
    EXPECT_EQ(popupOpacity(1.2F, 1.2F), 1.0F);
    EXPECT_EQ(popupOpacity(0.6F, 1.2F), 1.0F);
    EXPECT_NEAR(popupOpacity(0.3F, 1.2F), 0.5F, 0.001F);
    EXPECT_EQ(popupOpacity(0.0F, 1.2F), 0.0F);
    EXPECT_EQ(popupOpacity(-0.1F, 1.2F), 0.0F);
    EXPECT_EQ(popupOpacity(0.5F, 0.0F), 0.0F);
}

TEST(components_Popup, risesWithTime)
{
    EXPECT_EQ(popupRise(1.2F, 1.2F), 0.0F);
    EXPECT_NEAR(popupRise(0.2F, 1.2F), POPUP_RISE, 0.001F);
    EXPECT_EQ(popupRise(1.5F, 1.2F), 0.0F);  // never below its start
}

TEST(components_Popup, noAnchorByDefault)
{
    Popup popup{ .text = "+1 LIFE" };

    EXPECT_FALSE(popup.anchor.has_value());
}
