/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderCharge system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    /**
     * Draws the charge animation in front of every ship
     * charging a shot: the local player's from its own
     * trigger (LocalCharge), the others' from PlayerStats.
     */
    class RenderCharge {
        void* m_Engine;
        const Render::Renderer* m_Renderer;
        Render::Resources* m_Resources;
        float m_Elapsed;  // plays the animation

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 5,  // over the ships, their outline and names
            };

            RenderCharge() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Protecs::Components::PlayerStats>())
                    .set(Components::Manager::index<Components::Sprite>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }
            inline void setResources(Render::Resources& resources) noexcept { m_Resources = &resources; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
