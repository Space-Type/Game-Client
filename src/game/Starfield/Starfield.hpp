/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** starfield entities
*/

#pragma once

#include "../Engine.hpp"
#include <rtype/render.hpp>
#include <cstdint>


namespace RType::Game {
    /**
     * Creates the stars scrolling from right to left behind the
     * game (Components::Star, moved by Systems::ScrollStars and
     * drawn by Systems::RenderStarfield).
     *
     * Pure decoration, so client-only. Three layers move at
     * different speeds (parallax): the far ones are many,
     * small, dim and slow. The stars are spread over `area`,
     * always placed the same way for a given seed.
     */
    void spawnStarfield(Engine& engine, Render::Vec2D area, std::uint32_t seed = 1);
}
