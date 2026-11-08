/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PlayMusic system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>
#include <string>


namespace RType::Systems {
    /**
     * Plays the soundtrack (an entity with Music): starts its
     * track when it changes, in a loop, and streams it every
     * frame. A track that cannot be loaded is silence.
     */
    class PlayMusic {
        void* m_Engine;
        Render::Audio* m_Audio;
        std::string m_Playing;  // track being played, "" for none

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 1,
            };

            PlayMusic() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Music>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setAudio(Render::Audio& audio) noexcept { m_Audio = &audio; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };
}
