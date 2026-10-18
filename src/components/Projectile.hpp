/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Projectile Component
*/

#pragma once


namespace RType::Components {
    struct Projectile {
        float velocityX;  // virtual pixels per second, negative = goes left
        int damage = 1;
    };
}
