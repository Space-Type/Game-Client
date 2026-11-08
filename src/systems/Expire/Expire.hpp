/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Expire system
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    /**
     * Counts down the Lifetime of client-only entities and
     * deletes each one when it runs out. Only the current
     * entity is deleted: the engine keeps iterating safely.
     */
    class Expire {
        void* m_Engine;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 3,
            };

            Expire() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Lifetime>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
