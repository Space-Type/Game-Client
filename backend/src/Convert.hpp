/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Private header for
** backend to raylib conversions
*/

#pragma once

// Backend headers must come before raylib.h:
// raylib defines color macros (BLACK, WHITE...)
// that clash with Backend::Colors. Never use those
// macros in backend code either: inside namespace
// Backend they expand to Backend::Color, not ::Color
#include <backend/Types.hpp>
#include <raylib.h>

namespace Backend::Convert {

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
