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
     * Time since the server last sent one of the entity's
     * components.
     *
     * The server sends every shared component 20 times per
     * second and deletes an entity with a single EntityDelete. If
     * that packet is lost, or handled before the EntityCreate
     * it follows, the client would keep the entity frozen
     * forever: an entity the server stopped updating is gone.
     * (The level entity has no Transform, but its Level comes
     * like any other component.)
     */
    struct Freshness {
        float sinceUpdate = 0.0F;  // seconds
    };

    inline constexpr float STALE_AFTER = 1.0F;    // 20 updates missed
    inline constexpr float NETWORK_ALIVE = 0.5F;  // components keep coming for other entities

    /**
     * Only while components keep coming for other entities:
     * a server or network hiccup must not delete everything.
     */
    constexpr bool isStale(float sinceUpdate, float sinceAnyUpdate) noexcept
    {
        return sinceUpdate > STALE_AFTER && sinceAnyUpdate < NETWORK_ALIVE;
    }
}
