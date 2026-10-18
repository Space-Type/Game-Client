/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Projectile Component
*/

#pragma once

#include <rtype/render.hpp>


namespace RType::Components {
    struct Projectile {
        float velocityX;  // virtual pixels per second, negative = goes left
        int damage = 1;
        Render::SoundId hitSound = Render::SoundId::Invalid;  // played on impact, Invalid = silent
    };
}
