/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Shooter Component
*/

#pragma once

#include <rtype/render.hpp>


namespace RType::Components {
    struct Shooter {
        Render::TextureId bulletTexture;
        Render::SoundId shotSound = Render::SoundId::Invalid;  // Invalid = silent
        float cooldown = 0.25f;      // seconds between two shots
        float timer = 0.0f;          // seconds before the next shot is allowed
        float bulletSpeed = 500.0f;  // virtual pixels per second, negative = shoots left
        int damage = 1;
        bool autoFire = false;       // shoots whenever possible (enemies)
        bool wantsToShoot = false;   // set by Controllable for players
    };
}
