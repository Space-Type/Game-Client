/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Session class
*/

#pragma once

#include "../Engine.hpp"
#include "../Client/Client.hpp"
#include "../Controls/Controls.hpp"
#include "../DisplayOptions/DisplayOptions.hpp"
#include "../Starfield/Starfield.hpp"
#include <rtype/ui.hpp>
#include <rtype/render.hpp>
#include <rtype/utils.hpp>
#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <vector>


namespace RType::Game {
    /**
     * One networked game: the connection to the
     * server and the local copy of its entities.
     *
     * Never copied nor moved: the network
     * thread of the client refers to it.
     */
    class Session {
        static constexpr unsigned int MAX_EVENTS_PER_UPDATE = 100;

        Render::Resources& m_Resources;
        const Render::Renderer& m_Renderer;
        Render::Audio& m_Audio;
        Starfield m_Starfield;

        // Sounds of the server's events (Invalid without an audio device: then silent)
        Render::SoundId m_PlayerShotSound;
        Render::SoundId m_EnemyShotSound;
        Render::SoundId m_ExplosionSound;
        Render::SoundId m_ChargeSound;
        Render::SoundId m_BeamSound;
        bool m_ChargeSoundPlaying = false;

        // Client-only entities (explosions), deleted once their animation is over
        std::vector<Wrecks::ECS::Entity> m_Effects;
        Engine m_Engine;
        Utils::EntityMap m_EntityMap;

        Protecs::Tick m_SimulationTick;
        Protecs::Tick m_ServerTickDelay;
        Wrecks::DeltaTime m_AccumulatedDelta;

        bool m_Ended;
        std::string m_EndReason;
        std::optional<float> m_GameOverIn;  // seconds left before ending, once the local player is out

        Client m_Client;  // last: its network thread stops first
        std::chrono::steady_clock::time_point m_LastSyncTime;

        public:
            static constexpr float GAME_OVER_DELAY = 1.5F;  // lets the last explosion play
            static constexpr float HIT_BLINK = 0.2F;         // an enemy hit but not killed blinks as long

            Session(Render::Resources& resources, const Render::Renderer& renderer, Render::Audio& audio,
                    const Render::Input& input, const Controls& controls, const DisplayOptions& display);
            ~Session();

            Session(const Session&) = delete;
            Session(Session&&) = delete;
            Session& operator=(const Session&) = delete;
            Session& operator=(Session&&) = delete;

            // See Client::beginConnect and Client::pollConnect
            void beginConnect(const Protecs::Networking::Address& server, std::string_view name,
                              std::chrono::milliseconds timeout = Client::CONNECT_TIMEOUT);
            bool pollConnect();

            void update(Wrecks::DeltaTime dt);
            void render(Wrecks::DeltaTime dt);

            void setControlsEnabled(bool enabled) noexcept;

            // Players with their stats, the local one first then by slot
            [[ nodiscard ]] std::vector<UI::HudLine> players() const;

            [[ nodiscard ]] inline bool ended() const noexcept { return m_Ended; }
            [[ nodiscard ]] inline std::string_view endReason() const noexcept { return m_EndReason; }

        private:
            void handleEvents(Protecs::Tick tick);

            void initPlayer(Wrecks::ECS::Entity entity, Protecs::NetworkEntity id);
            void explode(Wrecks::ECS::Entity entity);
            void loseLife(Wrecks::ECS::Entity player, std::uint8_t livesLeft);
            void removeFinishedEffects();
            void updateChargeSound();
            void initSprite(Wrecks::ECS::Entity entity, Protecs::Networking::Packets::EntityCreate::EntityType type);

        private:
            struct EventHandler {
                Session& session;

                void operator()(const Protecs::Networking::Packets::EntityComponentData& data);
                void operator()(const Protecs::Networking::Packets::EntityCreate& data);
                void operator()(const Protecs::Networking::Packets::EntityDelete& data);
                void operator()(const Protecs::Networking::Packets::EntityList& data);
                void operator()(const Protecs::Networking::Packets::EntityRemoveComponent& data);
                void operator()(const Protecs::Networking::Packets::EntitySyncComponents& data);
                void operator()(const Protecs::Networking::Packets::Ping& data);
                void operator()(const Protecs::Networking::Packets::PlayerId& data);
                void operator()(const Protecs::Networking::Packets::PlayerInput& data);
                void operator()(const Protecs::Networking::Packets::PlayerJoin& data);
                void operator()(const Protecs::Networking::Packets::PlayerLeave& data);
                void operator()(const Protecs::Networking::Packets::TickSync& data);
            };
    };
}
