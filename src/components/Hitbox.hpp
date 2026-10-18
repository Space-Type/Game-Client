/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Hitbox Component
*/

#pragma once


namespace RType::Components {
    enum class Team {
        Player,
        Enemy,
    };

    struct Hitbox {
        float width;   // virtual pixels
        float height;
        Team team;
    };
}
