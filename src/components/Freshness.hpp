/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Freshness component
*/

#pragma once


namespace RType::Components {
    /**
     * Time since the server last sent the entity's position.
     *
     * The server sends every position 20 times per second
     * and deletes an entity with a single EntityDelete. If
     * that packet is lost, or handled before the EntityCreate
     * it follows, the client would keep the entity frozen
     * forever: an entity the server stopped updating is gone.
     */
    struct Freshness {
        float sinceUpdate = 0.0F;  // seconds
    };

    inline constexpr float STALE_AFTER = 1.0F;    // 20 position updates missed
    inline constexpr float NETWORK_ALIVE = 0.5F;  // positions keep coming for other entities

    /**
     * Only while positions keep coming for other entities:
     * a server or network hiccup must not delete everything.
     */
    constexpr bool isStale(float sinceUpdate, float sinceAnyUpdate) noexcept
    {
        return sinceUpdate > STALE_AFTER && sinceAnyUpdate < NETWORK_ALIVE;
    }
}
