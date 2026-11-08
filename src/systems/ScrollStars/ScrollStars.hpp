/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ScrollStars system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>
#include <cstdint>
#include <random>


namespace RType::Systems {
    /**
     * Scrolls the stars to the left, depending only on the
     * elapsed time, never on the frame rate. A star leaving on
     * the left comes back on the right at a random height, so
     * no pattern repeats.
     */
    class ScrollStars {
        void* m_Engine;
        Render::Vec2D m_Area;
        std::minstd_rand m_Random;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 5,
            };

            ScrollStars() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Star>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setArea(Render::Vec2D area) noexcept { m_Area = area; }
            inline void seed(std::uint32_t seed) noexcept { m_Random.seed(seed); }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
