/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Star component
*/

#pragma once

#include <rtype/render.hpp>
#include <cmath>


namespace RType::Components {
    // One star of the scrolling starfield, client-only decoration (see Game::spawnStarfield)
    struct Star {
        Render::Vec2D position;
        float size = 1.0F;   // virtual pixels
        float speed = 0.0F;  // virtual pixels per second, to the left
        Render::Color color;
    };

    /**
     * Moves a star to the left for `seconds`. A star leaving on
     * the left comes back on the right (x stays in [0, width),
     * whatever the time step): returns true then, so that it
     * takes a new height.
     */
    inline bool scroll(Star& star, float seconds, float width) noexcept
    {
        float x = star.position.x - star.speed * seconds;

        if (x >= 0.0F || width <= 0.0F) {
            star.position.x = x;
            return false;
        }
        star.position.x = std::fmod(x, width);
        if (star.position.x < 0.0F)
            star.position.x += width;
        return true;
    }
}
