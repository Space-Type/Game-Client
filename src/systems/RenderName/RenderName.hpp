/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderName system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>
#include <string>
#include <string_view>


namespace RType::Systems {
    /**
     * Writes each player's nickname, small, above
     * their ship, so players are told apart by text
     * and not only by colour (accessibility).
     */
    class RenderName {
        void* m_Engine;
        const Render::Renderer* m_Renderer;
        const Render::Resources* m_Resources;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 3,  // over the sprites and their outline
            };

            static constexpr float TEXT_SIZE = 18.0F;
            static constexpr float MARGIN = 4.0F;   // around the text, and between it and the ship

            RenderName() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Protecs::Components::PlayerName>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }
            inline void setResources(const Render::Resources& resources) noexcept { m_Resources = &resources; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };

    // The name with every character the default font cannot draw (outside printable ASCII) replaced by '?'
    std::string printableName(std::string_view name);
}
