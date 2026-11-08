/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PlayChargeSound system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    // Plays the charge sound of the local ship while it charges, and stops it on release (after ChargeTimer)
    class PlayChargeSound {
        void* m_Engine;
        Render::Audio* m_Audio;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 2,
            };

            PlayChargeSound() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::LocalCharge>())
                    .set(Components::Manager::index<Components::ChargeSound>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setAudio(Render::Audio& audio) noexcept { m_Audio = &audio; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };
}
