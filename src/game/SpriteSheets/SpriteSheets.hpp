/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** sprites of the subject's sprite sheets
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <cstddef>
#include <cstdint>


namespace RType::Game {
    // Ship colours in r-typesheet42.gif: one row each, the 4 players take the first 4
    inline constexpr std::size_t PLAYER_COLOURS = 4;

    // Throw RType::Render::Error if a sheet cannot be loaded
    Components::Sprite playerSprite(Render::Resources& resources, std::size_t slot);
    Components::Sprite enemySprite(Render::Resources& resources);
    Components::Sprite playerMissileSprite(Render::Resources& resources);
    Components::Sprite enemyMissileSprite(Render::Resources& resources);
    Components::Sprite explosionSprite(Render::Resources& resources);  // played once
    Components::Sprite beamSprite(Render::Resources& resources, std::uint8_t level);  // charged shot, level 1 to 5
    Components::Sprite chargeSprite(Render::Resources& resources);  // in front of a charging ship
    Components::Sprite bossSprite(Render::Resources& resources);    // stage 1 boss body (its core is drawn in it)
    Components::Sprite tailSprite(Render::Resources& resources);    // a segment of the stage 1 boss tail
    Components::Sprite gomanderSprite(Render::Resources& resources);  // stage 2 boss (its eye, the core, is drawn in it)
    Components::Sprite lifePickupSprite(Render::Resources& resources);  // a bonus life dropped by an enemy
    Components::Sprite powArmorSprite(Render::Resources& resources);    // the enemy carrying a Force item
    Components::Sprite forcePickupSprite(Render::Resources& resources); // the Force item
    Components::Sprite forceSprite(Render::Resources& resources, std::uint8_t level);  // the Force, level 1 to 3

    // Moves a player sprite to the row of its colour (slot modulo PLAYER_COLOURS)
    void setPlayerColour(Components::Sprite& sprite, std::size_t slot) noexcept;
}
