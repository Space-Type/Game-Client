/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
#include "../Client/Client.hpp"
#include "../Controls/Controls.hpp"
#include <rtype/render.hpp>


namespace RType::Game {
    using std::chrono_literals::operator""ms;

    class Game {
        static constexpr double FIXED_UPDATES_PER_FPS = 2.0;

        Render::Window m_Window;  // first: outlives everything that depends on it
        Render::Resources m_Resources;
        Render::Audio m_Audio;
        Render::Renderer m_Renderer;
        Render::Input m_Input;
        Controls m_Controls;
        Engine m_Engine;

        Wrecks::DeltaTime m_TargetDuration;
        Wrecks::DeltaTime m_FixedUpdateRate;
        Wrecks::DeltaTime m_AccumulatedDelta;

        Utils::EntityMap m_EntityMap;
        Client m_Client;

        public:
            Game();

            inline void stop() noexcept { m_Window.close(); }
            void setFPS(double targetFPS) noexcept;

            void run();

        private:
            void update(Wrecks::DeltaTime delta);
            void handleEvents();

            void initPlayer(Wrecks::ECS::Entity entity);
            void initEnemyPataPata(Wrecks::ECS::Entity entity);

        private:
            struct EventHandler {
                Game& game;

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
            };
    };
}
