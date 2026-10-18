/*
** EPITECH PROJECT, 2026
** Game-Client
** File description:
** HasRandomMovement.hpp
*/

#pragma once


namespace RType::Components {
    struct HasRandomMovement {
        int directionY = 0;   // -1 = monte, 0 = tout droit, 1 = descend
        float timer = 0.0f;   // seconds before picking a new direction
        float speed = 120.0f; // virtual pixels per second
    };
}
