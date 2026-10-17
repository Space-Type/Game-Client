/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Renderer class
*/

#pragma once

#include "../Types.hpp"
#include <wrecks/common.hpp>

namespace RType::Render {

    class Resources;

    /**
     * 2D drawing in virtual coordinates.
     *
     * Draw calls are only valid between
     * `Window::beginFrame()` and `Window::endFrame()`.
     * Unknown handles are ignored (nothing drawn).
     * Text must be null-terminated (`Wrecks::cstring_view`).
     */
    class Renderer {
        Resources& m_Resources;

        public:
            explicit Renderer(Resources& resources) noexcept;

            void drawSprite(TextureId texture, const SpriteParams& params) const;

            /**
             * Tiles `texture` over `dest`, scaled by `scale`,
             * shifted by `offset` (texture pixels) with wrap-around.
             * Increasing `offset.x` over time scrolls the texture
             * left: used for the star-field background.
             */
            void drawScrolling(TextureId texture, Rect dest, Vec2 offset,
                               float scale = 1.0F, Color tint = Colors::WHITE) const;

            void drawRect(Rect rect, Color color) const;
            void drawRectLines(Rect rect, float thickness, Color color) const;
            void drawCircle(Vec2 center, float radius, Color color) const;
            void drawLine(Vec2 start, Vec2 end, float thickness, Color color) const;

            // `position` is the top-left corner, `size` the font height
            void drawText(Wrecks::cstring_view text, Vec2 position, float size,
                          Color color, FontId font = FontId::Default) const;
            Vec2 measureText(Wrecks::cstring_view text, float size,
                             FontId font = FontId::Default) const;
    };
}
