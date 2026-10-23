/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Starfield System
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    /**
     * Scrolls the stars to the left; a star leaving
     * the screen comes back on the right at a random height.
     */
    class Starfield {
        void* m_Engine;
        const RType::Render::Window* m_Window;
        const float* m_DeltaTime;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::STATE };

            Starfield() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Star>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setDeltaTime(const float& deltaTime) noexcept { m_DeltaTime = &deltaTime; }
            inline void setWindow(const RType::Render::Window& window) noexcept { m_Window = &window; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
