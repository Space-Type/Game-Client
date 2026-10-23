/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** StarRender System
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class StarRender {
        void* m_Engine;
        const RType::Render::Renderer* m_Renderer;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::RENDER };

            StarRender() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Star>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const RType::Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
