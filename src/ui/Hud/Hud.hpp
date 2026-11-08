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
#include <string_view>
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
     * Above HEART_STACK_LIMIT lives, one heart and "xN"
     * keep the line short. Hearts are green, then orange,
     * then red and blinking on the last life; their number
     * carries the lives, so colour is never the only cue.
     */
    class Hud {
        public:
            static constexpr float ORIGIN = 10.0F;
            static constexpr float ROW_HEIGHT = 26.0F;
            static constexpr float TEXT_SIZE = 18.0F;
            static constexpr float PADDING = 6.0F;
            static constexpr float GAP = 12.0F;
            static constexpr float HEART_SCALE = 2.0F;
            static constexpr std::uint8_t HEART_STACK_LIMIT = 5;  // above it: one heart and "xN"
            static constexpr std::size_t NAME_MAX_LENGTH = 12;
            static constexpr float LAST_LIFE_BLINK_PERIOD = 0.5F;  // 2 blinks per second
            static constexpr float BOSS_BAR_WIDTH = 600.0F;
            static constexpr float BOSS_BAR_HEIGHT = 14.0F;
            static constexpr float FADE_IN = 0.3F;   // seconds
            static constexpr float FADE_OUT = 0.7F;

            // Draws the lines (local player first is up to the caller)
            static void draw(const Render::Renderer& renderer, const Render::Resources& resources, Render::TextureId heart,
                             const std::vector<HudLine>& lines, float seconds);

            // Boss health at the bottom of the screen: name, bar and "hp / max" (not only the bar)
            static void drawBossBar(const Render::Renderer& renderer, Render::Vec2D screen, std::string_view name, std::uint16_t hp, std::uint16_t max);
            static std::string bossLabel(std::string_view name, std::uint16_t hp, std::uint16_t max);  // "NAME  hp / max"
            static float bossBarFill(std::uint16_t hp, std::uint16_t max, float width) noexcept;

            // Announcements fade in, stay, then fade out: their opacity from 0 to 1
            static float announcementOpacity(float elapsed, float duration) noexcept;

            static Render::Color heartColor(std::uint8_t lives) noexcept;
            static std::uint8_t heartCount(std::uint8_t lives) noexcept;  // hearts drawn
            static std::string livesLabel(std::uint8_t lives);            // "xN" after them, empty up to HEART_STACK_LIMIT
            static bool heartsVisible(std::uint8_t lives, float seconds) noexcept;
            static std::string scoreText(std::uint32_t score);
    };
}
