/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Physics System
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class Physics {
        void* m_Engine;
        const RType::Render::Window* m_Window;
        const float* m_DeltaTime;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::STATE };

            Physics() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::Physics>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setDeltaTime(const float& deltaTime) noexcept { m_DeltaTime = &deltaTime; }
            inline void setWindow(const RType::Render::Window& window) noexcept { m_Window = &window; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
