/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Sprite component
*/

#pragma once

#include <rtype/render.hpp>
#include <cstddef>
#include <optional>


namespace RType::Components {
    /**
     * A texture, or frames of a sprite sheet. Frames
     * are side by side, `frame->width` apart, starting
     * at `frame`. They play at `framesPerSecond`, in a
     * loop or once (then the last frame stays).
     */
    struct Sprite {
        Render::TextureId texture = Render::TextureId::Invalid;
        std::optional<Render::Rect> frame = {};  // first frame, the whole texture if empty
        std::size_t frameCount = 1;
        float framesPerSecond = 0.0F;            // 0: no animation
        bool loop = true;
        float elapsed = 0.0F;                    // seconds, advanced by the RenderSprite system
    };

    // Frame shown after `sprite.elapsed` seconds
    constexpr std::size_t frameIndex(const Sprite& sprite) noexcept
    {
        if (sprite.frameCount <= 1 || sprite.framesPerSecond <= 0.0F || sprite.elapsed <= 0.0F)
            return 0;

        auto played = static_cast<std::size_t>(sprite.elapsed * sprite.framesPerSecond);

        return sprite.loop ? played % sprite.frameCount : (played < sprite.frameCount ? played : sprite.frameCount - 1);
    }

    // Texture area of the frame `index` (nullopt: the whole texture)
    constexpr std::optional<Render::Rect> frameAt(const Sprite& sprite, std::size_t index) noexcept
    {
        if (!sprite.frame)
            return std::nullopt;

        Render::Rect rect = *sprite.frame;

        rect.x += rect.width * static_cast<float>(index);
        return rect;
    }

    // True once a sprite played once has shown its last frame for a full frame time
    constexpr bool finished(const Sprite& sprite) noexcept
    {
        return !sprite.loop && sprite.framesPerSecond > 0.0F
            && sprite.elapsed * sprite.framesPerSecond >= static_cast<float>(sprite.frameCount);
    }
}
