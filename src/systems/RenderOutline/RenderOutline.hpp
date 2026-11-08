/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderOutline system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include "../../game/DisplayOptions/DisplayOptions.hpp"
#include <wrecks.hpp>


namespace RType::Systems {
    /**
     * High contrast mode: draws a thick outline around
     * every player and enemy, white for players and
     * orange for enemies, over their sprite. The box
     * is the sprite's, or a default one for an entity
     * without a sprite, so it is never invisible.
     */
    class RenderOutline {
        void* m_Engine;
        const Render::Renderer* m_Renderer;
        const Render::Resources* m_Resources;
        const Game::DisplayOptions* m_Options;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 3,  // over the sprites
            };

            // Virtual pixels, before the entity's scale
            static constexpr float DEFAULT_SIZE = 48.0F;

            RenderOutline() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::EntityKind>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }
            inline void setResources(const Render::Resources& resources) noexcept { m_Resources = &resources; }
            inline void setOptions(const Game::DisplayOptions& options) noexcept { m_Options = &options; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };
}
