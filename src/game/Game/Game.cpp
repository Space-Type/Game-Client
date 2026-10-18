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
    : m_Window({.title = "Spaceship demo", .width = 800, .height = 450, .virtualWidth = 800, .virtualHeight = 450})
    , m_Resources(m_Window)
    , m_Renderer(m_Resources)
    , m_Input(m_Window)
    , m_Engine()
    , m_IsRunning(false)
    , m_FrameDuration(1.0 / 120)  // 120 FPS
{
    m_Engine.get<Systems::Render>().setRenderer(m_Renderer);
    m_Engine.get<Systems::Controllable>().setInput(m_Input);

    auto spaceship = m_Engine.createEntity();
    auto ennemies = m_Engine.createEntity();

    m_Engine.addComponent(spaceship, RType::Components::Transform{ .x = 0.0, .y = 0.0, .rotation = 0.0 });
    m_Engine.addComponent(spaceship, RType::Components::Physics{ .velocityX = 1.0, .velocityY = 0.0 });
    m_Engine.addComponent(spaceship, RType::Components::Controllable{});
    m_Engine.addComponent(spaceship, RType::Components::Sprite{ .texture = m_Resources.loadTexture("../assets/images.png") });

    m_Engine.addComponent(ennemies, RType::Components::Transform{.x = 800, .y = 200, .rotation = 2.0 });
    m_Engine.addComponent(ennemies, RType::Components::HasRandomMovement{});
    m_Engine.addComponent(ennemies, RType::Components::Sprite{ .texture = m_Resources.loadTexture("../assets/pizza.png") });

}

void RType::Game::Game::run()
{
    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    m_IsRunning = true;

    while (m_IsRunning) {
        m_Window.beginFrame(Render::Colors::WHITE);
        m_Engine.update();
        m_Window.endFrame();
        m_IsRunning = !m_Window.shouldClose();  // TODO: remove

        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();

        Duration delta = frameEnd - frameStart;
        Duration sleepTime = std::max(m_FrameDuration - delta, Duration(0));
        std::this_thread::sleep_for(sleepTime);
        frameStart = frameEnd + std::chrono::duration_cast<std::chrono::nanoseconds>(sleepTime);
    }

}
