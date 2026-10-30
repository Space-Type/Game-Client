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
#include <memory>


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

        Wrecks::DeltaTime m_TargetDuration;
        Wrecks::DeltaTime m_FixedUpdateRate;

        std::unique_ptr<Session> m_Session;  // after the window and resources it uses

        public:
            Game();

            inline void stop() noexcept { m_Window.close(); }
            void setFPS(double targetFPS) noexcept;

            void run();

        private:
            void update(Wrecks::DeltaTime delta);
    };
}
