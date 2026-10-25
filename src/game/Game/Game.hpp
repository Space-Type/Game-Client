/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
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

        public:
            Game();

            inline void stop() noexcept { m_Window.close(); }
            void setFPS(double targetFPS) noexcept;

            void run();

        private:
            void update(Wrecks::DeltaTime delta);
            void createStarfield();
    };
}
