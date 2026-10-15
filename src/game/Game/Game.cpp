/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <chrono>
#include <thread>


RType::Game::Game::Game()
    : m_Engine()
    , m_IsRunning(false)
    , m_FrameDuration(1.0 / 120)  // 120 FPS
{
    auto spaceship = m_Engine.createEntity();

    m_Engine.addComponent(spaceship, RType::Components::Transform{ .x = 0.0, .y = 0.0, .rotation = 0.0 });
    m_Engine.addComponent(spaceship, RType::Components::Physics{ .velocityX = 10.0, .velocityY = 0.0 });
    m_FrameDuration = Duration(1);
}

void RType::Game::Game::run()
{
    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    m_IsRunning = true;

    while (m_IsRunning) {
        m_Engine.update();
        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();

        Duration delta = frameEnd - frameStart;
        Duration sleepTime = std::max(m_FrameDuration - delta, Duration(0));
        std::this_thread::sleep_for(sleepTime);
        frameStart = frameEnd + std::chrono::duration_cast<std::chrono::nanoseconds>(sleepTime);
    }
}
