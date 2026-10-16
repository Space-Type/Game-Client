/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Source file for
** letterbox math
*/

#include <backend/Letterbox.hpp>
#include <algorithm>

Backend::Rect Backend::computeViewport(Vec2 windowSize, Vec2 virtualSize) noexcept
{
    if (windowSize.x <= 0.0F || windowSize.y <= 0.0F || virtualSize.x <= 0.0F || virtualSize.y <= 0.0F)
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

Backend::Vec2 Backend::screenToVirtual(Vec2 screen, Rect viewport, Vec2 virtualSize) noexcept
{
    if (viewport.width <= 0.0F || viewport.height <= 0.0F)
        return {};

    return {
        .x = (screen.x - viewport.x) * virtualSize.x / viewport.width,
        .y = (screen.y - viewport.y) * virtualSize.y / viewport.height,
    };
}
