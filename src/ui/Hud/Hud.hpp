/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** in-game HUD
*/

#pragma once

#include <rtype/render.hpp>
#include <cstdint>
#include <string>
#include <vector>


namespace RType::UI {
    // One player in the HUD
    struct HudLine {
        std::string name;
        std::uint8_t lives;
        std::uint32_t score;
        bool local;  // the player of this client: first, with the focus marker
    };

    /**
     * Compact HUD in the top-left corner, one line per
     * player: marker, name, one heart per life, score.
     * Hearts are green, then orange, then red and blinking
     * on the last life; their number carries the lives,
     * so colour is never the only cue.
     */
    class Hud {
        public:
            static constexpr float ORIGIN = 10.0F;
            static constexpr float ROW_HEIGHT = 26.0F;
            static constexpr float TEXT_SIZE = 18.0F;
            static constexpr float PADDING = 6.0F;
            static constexpr float GAP = 12.0F;
            static constexpr float HEART_SCALE = 2.0F;
            static constexpr std::size_t NAME_MAX_LENGTH = 12;
            static constexpr float LAST_LIFE_BLINK_PERIOD = 0.5F;  // 2 blinks per second

            // Draws the lines (local player first is up to the caller)
            static void draw(const Render::Renderer& renderer, const Render::Resources& resources, Render::TextureId heart,
                             const std::vector<HudLine>& lines, float seconds);

            static Render::Color heartColor(std::uint8_t lives) noexcept;
            static bool heartsVisible(std::uint8_t lives, float seconds) noexcept;
            static std::string scoreText(std::uint32_t score);
    };
}
