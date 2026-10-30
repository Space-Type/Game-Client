/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** scene text helpers
*/

#pragma once

#include <rtype/render.hpp>
#include <wrecks/common.hpp>


namespace RType::Scenes {
    inline constexpr float TITLE_SIZE = 64.0F;
    inline constexpr float TEXT_SIZE = 32.0F;

    // Draws `text` horizontally centered on the virtual screen, its top at `y`
    inline void drawCenteredText(const Render::Renderer& renderer, const Render::Window& window,
                                 Wrecks::cstring_view text, float y, float size,
                                 Render::Color color = Render::Colors::WHITE)
    {
        float width = renderer.measureText(text, size).x;

        renderer.drawText(text, { .x = (window.virtualSize().x - width) / 2.0F, .y = y }, size, color);
    }
}
