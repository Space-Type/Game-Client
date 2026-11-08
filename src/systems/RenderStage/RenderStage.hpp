/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderStage system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>
#include <cstdint>


namespace RType::Systems {
    /**
     * Draws the background of the level's stage
     * (assets/stages/stage-<n>.png), scrolled by
     * Level::progression and scaled to the screen height.
     * Drawn over the starfield: its pure black is made
     * transparent, so the stars only show in open space,
     * never on the walls and indoor areas.
     */
    class RenderStage {
        void* m_Engine;
        const Render::Renderer* m_Renderer;
        Render::Resources* m_Resources;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 0,  // behind every entity
            };

            static constexpr std::uint8_t STAGES = 8;

            RenderStage() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Level>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }
            inline void setResources(Render::Resources& resources) noexcept { m_Resources = &resources; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };

    /**
     * Part of a background of `texture` size seen on a `screen`
     * once scaled to the screen height, after `progression`
     * pixels: it stops at the end of the background.
     */
    Render::Rect stageView(float progression, Render::Vec2D texture, Render::Vec2D screen) noexcept;
}
