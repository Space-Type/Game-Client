/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** UI theme
*/

#pragma once

#include <rtype/render.hpp>


namespace RType::UI::Theme {
    // Light text on a dark background: contrast of at least 4.5:1 (see tests/ui/Theme.cpp)
    inline constexpr Render::Color BACKGROUND = { .r = 20, .g = 20, .b = 30, .a = 255 };
    inline constexpr Render::Color FOREGROUND = { .r = 235, .g = 235, .b = 235, .a = 255 };
    inline constexpr Render::Color BORDER_COLOR = { .r = 150, .g = 150, .b = 160, .a = 255 };
    inline constexpr Render::Color FOCUS_COLOR = { .r = 255, .g = 255, .b = 255, .a = 255 };
    inline constexpr Render::Color ERROR_COLOR = { .r = 255, .g = 170, .b = 60, .a = 255 };
    inline constexpr Render::Color VICTORY_COLOR = { .r = 90, .g = 220, .b = 100, .a = 255 };

    inline constexpr float TEXT_SIZE = 32.0F;
    inline constexpr float HINT_SIZE = 24.0F;
    inline constexpr float PADDING = 12.0F;

    // The focus shows without colour: thick border and marker
    inline constexpr float BORDER_THICKNESS = 2.0F;
    inline constexpr float FOCUS_THICKNESS = 6.0F;
    inline constexpr const char* FOCUS_MARKER = ">";
    inline constexpr float MARKER_GAP = 30.0F;

    // Room kept on the left of a text field for its label
    inline constexpr float LABEL_WIDTH = 280.0F;

    // Slider: the filled part stands out from the track and the background
    inline constexpr Render::Color TRACK_COLOR = { .r = 60, .g = 60, .b = 75, .a = 255 };
    inline constexpr Render::Color FILL_COLOR = { .r = 90, .g = 160, .b = 255, .a = 255 };
    inline constexpr float TRACK_HEIGHT = 12.0F;
    inline constexpr float VALUE_WIDTH = 100.0F;  // room on the right of the track for the value

    // HUD hearts, by lives left: they stand out from the background (non-text contrast)
    inline constexpr Render::Color HEART_HIGH = { .r = 80, .g = 200, .b = 90, .a = 255 };
    inline constexpr Render::Color HEART_MEDIUM = { .r = 255, .g = 160, .b = 40, .a = 255 };
    inline constexpr Render::Color HEART_LOW = { .r = 235, .g = 60, .b = 60, .a = 255 };

    // Checkbox: a square, crossed when checked
    inline constexpr float CHECK_SIZE = 28.0F;
    inline constexpr float CHECK_INSET = 6.0F;
    inline constexpr float CHECK_THICKNESS = 4.0F;
}
