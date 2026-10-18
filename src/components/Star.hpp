/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Star Component
*/

#pragma once

#include <rtype/render.hpp>


namespace RType::Components {
    struct Star {
        float speed;          // virtual pixels per second, towards the left
        float size;           // side of the square, in virtual pixels
        Render::Color color;  // dimmer for the far layers
    };
}
