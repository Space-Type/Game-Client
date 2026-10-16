/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Render System
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class Render {
        void* m_Engine;

        public:
            Render() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::Sprite>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
