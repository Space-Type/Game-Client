/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Game class
*/

#pragma once

#include "../Engine.hpp"
#include <chrono>
#include <ctime>


namespace RType::Game {
    class Game {
        using Duration = std::chrono::duration<double, std::milli>;

        Engine m_Engine;
        bool m_IsRunning;
        Duration m_FrameDuration;

        public:
            Game();

            inline void stop() noexcept { m_IsRunning = false; }
            inline void setFPS(double targetFPS) noexcept { m_FrameDuration = std::chrono::duration<double>(1.0 / targetFPS); }

            void run();
    };
}
