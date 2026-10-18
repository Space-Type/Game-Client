/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Collision System
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    // Runs once per bullet and checks it against every entity that has Health
    class Collision {
        void* m_Engine;
        RType::Render::Audio* m_Audio;

        public:
            Collision() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::Hitbox>())
                    .set(Components::Manager::index<Components::Projectile>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setAudio(RType::Render::Audio& audio) noexcept { m_Audio = &audio; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
