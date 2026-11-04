/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Controls/Controls.hpp"
#include "../Session/Session.hpp"
#include <rtype/render.hpp>
#include <rtype/scenes.hpp>
#include <protecs/common.hpp>
#include <memory>


namespace RType::Game {
    using std::chrono_literals::operator""ms;

    class Game {
        static constexpr double FIXED_UPDATES_PER_FPS = 2.0;

        // Not black: the bars must stand out from the game's background
        static constexpr Render::Color LETTERBOX_COLOR = { .r = 40, .g = 40, .b = 40, .a = 255 };

        using SceneManager = Scenes::Manager<Scenes::Context,
                                             Scenes::Menu,
                                             Scenes::Connecting,
                                             Scenes::InGame,
                                             Scenes::Paused,
                                             Scenes::GameOver>;

        Render::Window m_Window;  // first: outlives everything that depends on it
        Render::Resources m_Resources;
        Render::Audio m_Audio;
        Render::Renderer m_Renderer;
        Render::Input m_Input;
        Controls m_Controls;

        Wrecks::DeltaTime m_TargetDuration;

        std::unique_ptr<Session> m_Session;  // after the window and resources it uses
        Scenes::Context m_Context;           // after everything it refers to
        SceneManager m_Scenes;

        public:
            static constexpr int VIRTUAL_WIDTH = 1280;
            static constexpr int VIRTUAL_HEIGHT = 720;

            // The game world stays Protecs' (shared with the server), scaled at render time
            static constexpr float WORLD_TO_SCREEN = VIRTUAL_WIDTH / Protecs::POSITION_MAX_X;

            Game();

            inline void stop() noexcept { m_Window.close(); }
            void setFPS(double targetFPS) noexcept;

            void run();

        private:
            void update(Wrecks::DeltaTime delta);
    };

    static_assert(Game::VIRTUAL_WIDTH * Protecs::POSITION_MAX_Y == Game::VIRTUAL_HEIGHT * Protecs::POSITION_MAX_X,
                  "the virtual screen and the game world must have the same aspect ratio");
}
