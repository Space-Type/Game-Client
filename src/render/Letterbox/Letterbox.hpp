/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** letterbox math
*/

#pragma once

#include "../Types.hpp"

namespace RType::Render {

    /**
     * Returns the largest rectangle with the aspect
     * ratio of `virtualSize`, centered in a window
     * of size `windowSize` (screen pixels).
     *
     * Returns an empty Rect if any dimension
     * is not strictly positive (e.g. minimized window).
     */
    Rect computeViewport(Vec2D windowSize, Vec2D virtualSize) noexcept;

    /**
     * Converts a screen position into virtual
     * coordinates, given the viewport returned
     * by `computeViewport()`.
     *
     * Positions outside the viewport map outside
     * [0, virtualSize] (not clamped). Returns {0, 0}
     * if the viewport is empty.
     */
    Vec2D screenToVirtual(Vec2D screen, Rect viewport, Vec2D virtualSize) noexcept;
}
