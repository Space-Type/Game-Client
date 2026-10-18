/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Physics Component
*/

#pragma once


namespace RType::Components {
    struct Physics {
        float velocityX = 0.0f;   // virtual pixels per second
        float velocityY = 0.0f;
        float maxSpeed = 240.0f;  // virtual pixels per second
        float damping = 14.0f;    // higher = stops faster (velocity loses ~14x its value per second)
    };
}
