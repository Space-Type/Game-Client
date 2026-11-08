/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ForgetStale system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/utils.hpp>
#include <wrecks.hpp>
#include <chrono>


namespace RType::Systems {
    /**
     * Deletes the entities the server stopped updating (see
     * Components::Freshness), from the engine and the entity
     * map. The local ship (LocalCharge) is never forgotten.
     */
    class ForgetStale {
        void* m_Engine;
        Utils::EntityMap* m_EntityMap;
        const std::chrono::steady_clock::time_point* m_LastComponentAt;  // any entity

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 4,
            };

            ForgetStale() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Freshness>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setEntityMap(Utils::EntityMap& map) noexcept { m_EntityMap = &map; }
            inline void setLastComponentAt(const std::chrono::steady_clock::time_point& at) noexcept { m_LastComponentAt = &at; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
