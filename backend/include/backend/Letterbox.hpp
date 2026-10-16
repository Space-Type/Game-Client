/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Header file for
** letterbox math
*/

#pragma once

#include "Types.hpp"

namespace Backend {

    /**
     * Returns the largest rectangle with the aspect
     * ratio of `virtualSize`, centered in a window
     * of size `windowSize` (screen pixels).
     *
     * Returns an empty Rect if any dimension
     * is not strictly positive (e.g. minimized window).
     */
    Rect computeViewport(Vec2 windowSize, Vec2 virtualSize) noexcept;

    /**
     * Converts a screen position into virtual
     * coordinates, given the viewport returned
     * by `computeViewport()`.
     *
     * Positions outside the viewport map outside
     * [0, virtualSize] (not clamped). Returns {0, 0}
     * if the viewport is empty.
     */
    Vec2 screenToVirtual(Vec2 screen, Rect viewport, Vec2 virtualSize) noexcept;
}
