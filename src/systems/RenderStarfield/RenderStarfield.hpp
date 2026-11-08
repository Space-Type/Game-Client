/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderStarfield system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    // Draws the stars, behind everything else (the stage background then hides them indoors)
    class RenderStarfield {
        void* m_Engine;
        const Render::Renderer* m_Renderer;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 0,  // behind the stage background and every entity
            };

            RenderStarfield() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Star>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
