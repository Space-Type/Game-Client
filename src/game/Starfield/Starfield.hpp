/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Starfield class
*/

#pragma once

#include <rtype/render.hpp>
#include <wrecks/common.hpp>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>


namespace RType::Game {
    /**
     * Stars scrolling from right to left behind the game.
     *
     * Pure decoration, so drawn by the client alone. Three
     * layers move at different speeds (parallax): the far
     * ones are many, small, dim and slow. Positions only
     * depend on the elapsed time, never on the frame rate.
     * A star leaving on the left comes back on the right
     * at a random height, so no pattern repeats.
     */
    class Starfield {
        public:
            struct Star {
                Render::Vec2D position;
                float size;     // virtual pixels
                float speed;    // virtual pixels per second
                Render::Color color;
            };

            // Stars spread over a `size` area, always placed and moved the same way for a given seed
            explicit Starfield(Render::Vec2D size, std::uint32_t seed = 1);

            void update(Wrecks::DeltaTime dt) noexcept;
            void draw(const Render::Renderer& renderer) const;

            [[ nodiscard ]] inline const std::vector<Star>& stars() const noexcept { return m_Stars; }

        private:
            Render::Vec2D m_Size;
            std::vector<Star> m_Stars;
            std::minstd_rand m_Random;  // seeded: the same stars for the same seed
    };
}
