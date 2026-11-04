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
}
