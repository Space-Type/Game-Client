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
#include <rtype/render.hpp>
#include <rtype/utils.hpp>
#include <chrono>
#include <string>
#include <string_view>


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
        Engine m_Engine;
        Utils::EntityMap m_EntityMap;

        Protecs::Tick m_SimulationTick;
        Protecs::Tick m_ServerTickDelay;
        Wrecks::DeltaTime m_AccumulatedDelta;

        bool m_Ended;
        std::string m_EndReason;

        Client m_Client;  // last: its network thread stops first

        public:
            Session(Render::Resources& resources, const Render::Renderer& renderer,
                    const Render::Input& input, const Controls& controls);
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

            [[ nodiscard ]] inline bool ended() const noexcept { return m_Ended; }
            [[ nodiscard ]] inline std::string_view endReason() const noexcept { return m_EndReason; }

        private:
            void handleEvents(Protecs::Tick tick);

            void initPlayer(Wrecks::ECS::Entity entity);
            void initEnemyPataPata(Wrecks::ECS::Entity entity);

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
