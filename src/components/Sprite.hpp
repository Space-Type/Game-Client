/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Sprite Component
*/

#pragma once

#include <string_view>
#include <rtype/render.hpp>


namespace RType::Components {
    struct Sprite {
        Render::TextureId texture;
    };
}
