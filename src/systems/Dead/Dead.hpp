/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Dead System
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>
#include <vector>


namespace RType::Systems {
    class Dead {
        void* m_Engine;
        std::vector<Wrecks::ECS::Entity> m_Pending;

        public:
            Dead() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Dead>());
            }

            // Templated on the engine since Game::Engine depends on this header
            template <typename Engine>
            static bool isDead(const Engine& engine, Wrecks::ECS::Entity entity) noexcept
            {
                return engine.entitySignature(entity).test(Components::Manager::index<Components::Dead>());
            }

            template <typename Engine>
            static void kill(Engine& engine, Wrecks::ECS::Entity entity)
            {
                if (!isDead(engine, entity))
                    engine.addComponent(entity, Components::Dead{});
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity);

            // Deleting entities while the engine iterates over them is unsafe,
            // so they are only deleted once Engine::update() is done
            void flush();
    };
}
