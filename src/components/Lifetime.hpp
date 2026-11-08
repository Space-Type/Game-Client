/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Lifetime component
*/

#pragma once

#include "Sprite.hpp"


namespace RType::Components {
    /**
     * A client-only entity that deletes itself after a while
     * (explosions, announcements): see Systems::Expire.
     */
    struct Lifetime {
        float remaining = 0.0F;  // seconds
    };

    // Seconds a sprite played once takes to show all its frames (0 for a still or looping sprite)
    constexpr float animationDuration(const Sprite& sprite) noexcept
    {
        if (sprite.loop || sprite.framesPerSecond <= 0.0F)
            return 0.0F;
        return static_cast<float>(sprite.frameCount) / sprite.framesPerSecond;
    }
}
