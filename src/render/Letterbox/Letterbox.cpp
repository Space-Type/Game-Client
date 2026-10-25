/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** letterbox math
*/

#include "Letterbox.hpp"
#include <algorithm>

RType::Render::Rect RType::Render::computeViewport(Vec2D windowSize, Vec2D virtualSize) noexcept
{
    // Negated comparisons so NaN sizes are rejected too
    if (!(windowSize.x > 0.0F) || !(windowSize.y > 0.0F) || !(virtualSize.x > 0.0F) || !(virtualSize.y > 0.0F))
        return {};

    float scale = std::min(windowSize.x / virtualSize.x, windowSize.y / virtualSize.y);
    float width = virtualSize.x * scale;
    float height = virtualSize.y * scale;

    return {
        .x = (windowSize.x - width) / 2.0F,
        .y = (windowSize.y - height) / 2.0F,
        .width = width,
        .height = height,
    };
}

RType::Render::Vec2D RType::Render::screenToVirtual(Vec2D screen, Rect viewport, Vec2D virtualSize) noexcept
{
    if (viewport.width <= 0.0F || viewport.height <= 0.0F)
        return {};

    return {
        .x = (screen.x - viewport.x) * virtualSize.x / viewport.width,
        .y = (screen.y - viewport.y) * virtualSize.y / viewport.height,
    };
}
