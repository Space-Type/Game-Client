/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** UI theme
*/

#include <gtest/gtest.h>
#include <rtype/ui.hpp>
#include <algorithm>
#include <cmath>

namespace {

    using RType::Render::Color;
    using namespace RType::UI::Theme;

    double channel(std::uint8_t value)
    {
        double scaled = value / 255.0;

        return scaled <= 0.04045 ? scaled / 12.92 : std::pow((scaled + 0.055) / 1.055, 2.4);
    }

    double luminance(Color color)
    {
        return 0.2126 * channel(color.r) + 0.7152 * channel(color.g) + 0.0722 * channel(color.b);
    }

    // WCAG 2 contrast ratio
    double contrast(Color a, Color b)
    {
        double lighter = std::max(luminance(a), luminance(b));
        double darker = std::min(luminance(a), luminance(b));

        return (lighter + 0.05) / (darker + 0.05);
    }
}


TEST(ui_Theme, contrastIsAtLeastAA)
{
    for (Color background : {BACKGROUND, RType::Render::Colors::BLACK})
        for (Color color : {FOREGROUND, BORDER_COLOR, FOCUS_COLOR, ERROR_COLOR})
            EXPECT_GE(contrast(color, background), 4.5);
}

TEST(ui_Theme, focusShowsWithoutColor)
{
    EXPECT_GT(FOCUS_THICKNESS, BORDER_THICKNESS);
    EXPECT_STREQ(FOCUS_MARKER, ">");
}
