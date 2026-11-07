/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** ChargeTimer system
*/

#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    // Counts how long the local player holds the trigger (LocalCharge), after PlayerInput read it
    class ChargeTimer {
        void* m_Engine;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 1,
            };

            ChargeTimer() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::PlayerInput>())
                    .set(Components::Manager::index<Components::LocalCharge>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
