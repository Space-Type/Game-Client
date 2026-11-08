/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderPopup system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    // Draws the popups, centred on their position, rising and fading
    class RenderPopup {
        void* m_Engine;
        const Render::Renderer* m_Renderer;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 6,  // over everything in the game
            };

            static constexpr float TEXT_SIZE = 20.0F;

            RenderPopup() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Popup>())
                    .set(Components::Manager::index<Components::Lifetime>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };
}
