/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Private header for
** Render to raylib conversions
*/

#pragma once

// Render headers must come before raylib.h:
// raylib defines color macros (BLACK, WHITE...)
// that clash with RType::Render::Colors. Never use those
// macros in render code either: inside namespace
// RType::Render they expand to RType::Render::Color, not ::Color
#include "Types.hpp"
#include <raylib.h>

namespace RType::Render::Convert {

    inline ::Color toRaylib(Color color) noexcept
    {
        return {color.r, color.g, color.b, color.a};
    }

    inline ::Vector2 toRaylib(Vec2 vec) noexcept
    {
        return {vec.x, vec.y};
    }

    inline ::Rectangle toRaylib(Rect rect) noexcept
    {
        return {rect.x, rect.y, rect.width, rect.height};
    }

    inline Vec2 fromRaylib(::Vector2 vec) noexcept
    {
        return {vec.x, vec.y};
    }
}
