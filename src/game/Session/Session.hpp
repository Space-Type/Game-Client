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
#include <utility>
#include <vector>


namespace RType::Game {
    /**
     * One networked game: the connection to the
     * server and the local copy of its entities.
     *
     * Never copied nor moved: the network
     * thread of the client refers to it.
     */
    // Big text over the game: "LEVEL n" when a stage starts, "STAGE CLEAR" (with the scores) when it is won
    struct Announcement {
        std::string text;
        float start;      // Session clock
        float duration;   // seconds
        bool scores;      // also shows every player's score
    };

    // How a game ended: the GameOver scene shows a victory, a defeat (with the score) or a message
    enum class Outcome : std::uint8_t {
        Message,
        Victory,
        Defeat,
    };

    class Session {
        static constexpr unsigned int MAX_EVENTS_PER_UPDATE = 100;

        Render::Resources& m_Resources;
        Render::Audio& m_Audio;

        // Sounds of the server's events (Invalid without an audio device: then silent)
        Render::SoundId m_PlayerShotSound;
        Render::SoundId m_EnemyShotSound;
        Render::SoundId m_ExplosionSound;
        Render::SoundId m_ChargeSound;
        Render::SoundId m_BeamSound;
        Render::SoundId m_LevelSound;
        Render::SoundId m_StageClearSound;
        Render::SoundId m_WarningSound;

        float m_Clock = 0.0F;                     // seconds since the session started
        std::optional<std::uint8_t> m_Stage;      // last Level::stage received
        std::optional<std::uint8_t> m_BossPhase;  // last BossPhase::phase received
        std::optional<Announcement> m_Announcement;

        std::chrono::steady_clock::time_point m_LastComponentAt;  // last component received, for any entity (Freshness)
        Engine m_Engine;
        Utils::EntityMap m_EntityMap;

        Protecs::Tick m_SimulationTick;
        Protecs::Tick m_ServerTickDelay;
        Wrecks::DeltaTime m_AccumulatedDelta;

        bool m_Ended;
        std::string m_EndReason;            // Outcome::Message only
        Outcome m_Outcome = Outcome::Message;
        std::uint32_t m_FinalScore = 0;     // local player's score, for a victory or a defeat
        std::optional<float> m_GameOverIn;  // seconds left before ending, once the local player is out

        Client m_Client;  // last: its network thread stops first
        std::chrono::steady_clock::time_point m_LastSyncTime;

        public:
            static constexpr float GAME_OVER_DELAY = 1.5F;  // lets the last explosion play
            static constexpr float HIT_BLINK = 0.2F;         // an enemy hit but not killed blinks as long
            static constexpr float LEVEL_ANNOUNCEMENT = 2.5F;
            static constexpr float WARNING_ANNOUNCEMENT = 2.0F;  // a boss gets angry
            static constexpr float WARNING_BLINK = 1.0F;
            static constexpr float STAGE_CLEAR_ANNOUNCEMENT = 4.0F;  // as long as the server waits before the next stage (Server: STAGE_CLEAR_DELAY)

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

            // The announcement on screen, and how long it has been shown (seconds)
            [[ nodiscard ]] std::optional<std::pair<Announcement, float>> announcement() const;

            // Health of the boss core, while a boss is there
            [[ nodiscard ]] std::optional<Protecs::Components::Health> bossHealth() const;

            [[ nodiscard ]] inline bool ended() const noexcept { return m_Ended; }
            [[ nodiscard ]] inline std::string_view endReason() const noexcept { return m_EndReason; }
            [[ nodiscard ]] inline Outcome outcome() const noexcept { return m_Outcome; }
            [[ nodiscard ]] inline std::uint32_t finalScore() const noexcept { return m_FinalScore; }

        private:
            void handleEvents(Protecs::Tick tick);

            void initPlayer(Wrecks::ECS::Entity entity, Protecs::NetworkEntity id);
            void explode(Wrecks::ECS::Entity entity);
            void explodeAt(const Protecs::Components::Transform& where);
            void announce(std::string text, float duration, bool scores, Render::SoundId sound);
            void clearStage();
            void enrage();
            void win();
            [[ nodiscard ]] std::optional<Wrecks::ECS::Entity> findKind(Protecs::Networking::Packets::EntityCreate::EntityType type) const;
            [[ nodiscard ]] std::optional<Wrecks::ECS::Entity> localPlayer() const;
            void loseLife(Wrecks::ECS::Entity player, std::uint8_t livesLeft);
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
