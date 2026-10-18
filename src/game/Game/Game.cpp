/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <algorithm>
#include <chrono>
#include <thread>


RType::Game::Game::Game()
    : m_Window({.title = "Spaceship demo", .width = 800, .height = 450, .virtualWidth = 800, .virtualHeight = 450})
    , m_Resources(m_Window)
    , m_Renderer(m_Resources)
    , m_Input(m_Window)
    , m_Engine()
    , m_IsRunning(false)
    , m_FrameDuration(std::chrono::duration<double>(1.0 / 120))  // 120 FPS
    , m_DeltaTime(0.0f)
{
    m_Engine.get<Systems::Render>().setRenderer(m_Renderer);
    m_Engine.get<Systems::Controllable>().setInput(m_Input);
    m_Engine.get<Systems::Controllable>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::Physics>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::Physics>().setWindow(m_Window);
    m_Engine.get<Systems::RandomMovement>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::RandomMovement>().setWindow(m_Window);
    m_Engine.get<Systems::RandomMovement>().setResources(m_Resources);

    auto spaceship = m_Engine.createEntity();
    auto ennemies = m_Engine.createEntity();

    auto playerTexture = m_Resources.loadTexture("../assets/spaceship.png");
    int playerCount = 1;

    for (int i = 0; i < playerCount; ++i) {
        auto player = m_Engine.createEntity();

        m_Engine.addComponent(spaceship, RType::Components::Transform{ .x = 0.0, .y = 0.0, .rotation = 0.0 });
        m_Engine.addComponent(spaceship, RType::Components::Physics{});
        m_Engine.addComponent(spaceship, RType::Components::Controllable{});
        m_Engine.addComponent(spaceship, RType::Components::Sprite{ .texture = playerTexture });
    }

    auto enemyTexture = m_Resources.loadTexture("../assets/ennemi.png");
    int enemyCount = 5;

    for (int i = 0; i < enemyCount; ++i) {
        auto enemy = m_Engine.createEntity();

        m_Engine.addComponent(enemy, RType::Components::Transform{ .x = m_Window.virtualSize().x + i * 60.0f, .y = 50.0f + i * 70.0f, .rotation = 0.0f });
        m_Engine.addComponent(enemy, RType::Components::HasRandomMovement{ .directionY = 0, .timer = i * 0.2f });
        m_Engine.addComponent(enemy, RType::Components::Sprite{ .texture = enemyTexture });
    }
}

void RType::Game::Game::run()
{
    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    std::chrono::time_point lastFrame = frameStart;
    m_IsRunning = true;

    while (m_IsRunning) {
        std::chrono::time_point now = std::chrono::steady_clock::now();
        // Capped so a long stall (window drag, breakpoint) doesn't teleport entities
        m_DeltaTime = std::min(std::chrono::duration<float>(now - lastFrame).count(), 0.1f);
        lastFrame = now;

        m_Engine.update();
        m_Window.beginFrame(Render::Colors::WHITE);
        m_Engine.render();
        m_Window.endFrame();
        m_IsRunning = !m_Window.shouldClose();  // TODO: remove

        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();

        Duration delta = frameEnd - frameStart;
        Duration sleepTime = std::max(m_FrameDuration - delta, Duration(0));
        std::this_thread::sleep_for(sleepTime);
        frameStart = frameEnd + std::chrono::duration_cast<std::chrono::nanoseconds>(sleepTime);
    }

}
