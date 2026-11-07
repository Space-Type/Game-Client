/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** player state shown by the client
*/

#pragma once

#include <protecs/components.hpp>
#include <wrecks/ecs.hpp>
#include <cmath>


namespace RType::Components {
    // Same as the server's invulnerability after a life lost (Server: RESPAWN_INVULNERABILITY)
    inline constexpr float BLINK_DURATION = 2.0F;
    inline constexpr float BLINK_PERIOD = 0.2F;  // 5 blinks per second

    // A ship that just lost a life blinks while it cannot be hit
    struct Blink {
        float remaining = BLINK_DURATION;  // seconds, counted down by the RenderSprite system
    };

    // Hidden during the second half of each period, visible once the blink is over
    inline bool blinkVisible(float remaining) noexcept
    {
        return remaining <= 0.0F || std::fmod(remaining, BLINK_PERIOD) < BLINK_PERIOD / 2.0F;
    }

    // A player with no life left: no longer drawn
    template <typename Engine>
    bool isEliminated(Engine& engine, Wrecks::ECS::Entity entity)
    {
        return engine.template hasComponent<Protecs::Components::PlayerStats>(entity)
            && engine.template get<Protecs::Components::PlayerStats>()[entity].lives == 0;
    }
}
