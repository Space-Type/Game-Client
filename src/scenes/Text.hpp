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

    // Dims the session drawn under a menu
    inline constexpr Render::Color OVERLAY = { .r = 0, .g = 0, .b = 0, .a = 160 };

    inline void drawOverlay(const Render::Renderer& renderer, const Render::Window& window)
    {
        Render::Vec2D size = window.virtualSize();

        renderer.drawRect({ .x = 0.0F, .y = 0.0F, .width = size.x, .height = size.y }, OVERLAY);
    }

    // Draws `text` horizontally centered on the virtual screen, its top at `y`
    inline void drawCenteredText(const Render::Renderer& renderer, const Render::Window& window,
                                 Wrecks::cstring_view text, float y, float size,
                                 Render::Color color = Render::Colors::WHITE)
    {
        float width = renderer.measureText(text, size).x;

        renderer.drawText(text, { .x = (window.virtualSize().x - width) / 2.0F, .y = y }, size, color);
    }
}
