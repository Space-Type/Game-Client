/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Sprite Component
*/

#pragma once

#include <string_view>
#include <raylib.h>


namespace RType::Components {
    struct Sprite {
        Texture2D texture;

        Sprite(std::string_view path)
            : texture(LoadTexture(path.data()))
        {
        }

        ~Sprite()
        {
            UnloadTexture(texture);
        }
    };
}
