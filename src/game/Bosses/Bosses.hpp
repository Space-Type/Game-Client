/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** the bosses the client knows
*/

#pragma once

#include <protecs/networking.hpp>
#include <array>
#include <string_view>


namespace RType::Game {
    /**
     * A boss: its core (the weak point, whose Health is the
     * boss health and whose death clears the stage) and its
     * body (the big sprite, which blinks when the core is hit
     * and blows up all over when the boss dies).
     */
    struct BossKinds {
        Protecs::Networking::Packets::EntityCreate::EntityType core;
        Protecs::Networking::Packets::EntityCreate::EntityType body;
        std::string_view name;  // over its health bar
    };

    inline constexpr std::array<BossKinds, 2> BOSSES = {{
        { Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS_CORE, Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS, "DOBKERATOPS" },
        { Protecs::Networking::Packets::EntityCreate::ENEMY_GOMANDER_CORE, Protecs::Networking::Packets::EntityCreate::ENEMY_GOMANDER, "GOMANDER" },
    }};

    // The boss whose core is `type`, nullptr if it is not a boss core
    constexpr const BossKinds* bossOf(Protecs::Networking::Packets::EntityCreate::EntityType type) noexcept
    {
        for (const auto& boss : BOSSES)
            if (boss.core == type)
                return &boss;
        return nullptr;
    }

    constexpr bool isBossBody(Protecs::Networking::Packets::EntityCreate::EntityType type) noexcept
    {
        for (const auto& boss : BOSSES)
            if (boss.body == type)
                return true;
        return false;
    }
}
