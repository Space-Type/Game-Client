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
#include <chrono>
#include <ctime>


namespace RType::Game {
    class Game {
        using Duration = std::chrono::duration<double, std::milli>;

        Render::Window m_Window;  // first: outlives everything that depends on it
        Render::Resources m_Resources;
        Render::Audio m_Audio;
        Render::Renderer m_Renderer;
        Render::Input m_Input;
        Controls m_Controls;
        Engine m_Engine;
        Duration m_FrameDuration;
        float m_DeltaTime;  // seconds elapsed since the previous frame

        public:
            Game();
            ~Game();

            inline void stop() noexcept { m_Window.close(); }
            inline void setFPS(double targetFPS) noexcept { m_FrameDuration = std::chrono::duration<double>(1.0 / targetFPS); }

            void run();

        private:
            void createStarfield();
    };
}
