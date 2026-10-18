/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Shoot System
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>
#include <vector>


namespace RType::Systems {
    class Shoot {
        struct BulletRequest {
            Components::Transform transform;
            Components::Hitbox hitbox;
            Components::Projectile projectile;
            RType::Render::TextureId texture;
        };

        void* m_Engine;
        const RType::Render::Resources* m_Resources;
        RType::Render::Audio* m_Audio;
        const float* m_DeltaTime;
        std::vector<BulletRequest> m_Pending;

        public:
            Shoot() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::Hitbox>())
                    .set(Components::Manager::index<Components::Shooter>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setDeltaTime(const float& deltaTime) noexcept { m_DeltaTime = &deltaTime; }
            inline void setResources(const RType::Render::Resources& resources) noexcept { m_Resources = &resources; }
            inline void setAudio(RType::Render::Audio& audio) noexcept { m_Audio = &audio; }

            void operator()(Wrecks::ECS::Entity entity);

            // Creating entities while the engine iterates over them is unsafe,
            // so bullets are only created once Engine::update() is done
            void flush();
    };
}
