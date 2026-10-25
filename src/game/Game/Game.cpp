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
    , m_Audio()
    , m_Renderer(m_Resources)
    , m_Input(m_Window)
    , m_Controls(defaultControls())
    , m_Engine()
    , m_TargetDuration(std::chrono::duration<double>(1.0 / 120))  // 120 FPS
{
    m_Engine.get<Systems::RenderSprite>().setRenderer(m_Renderer);
    m_Engine.get<Systems::Controllable>().setInput(m_Input);
    m_Engine.get<Systems::Controllable>().setControls(m_Controls);

    m_Resources.loadTexture(Render::assetPath("spaceship.png"));
    m_Resources.loadTexture(Render::assetPath("blueshoot.png"));
    m_Audio.loadSound(Render::assetPath("sounds/playershoot.wav"));
    m_Audio.loadSound(Render::assetPath("sounds/enemy_hit.wav"));

    m_Resources.loadTexture(Render::assetPath("ennemi.png"));
    m_Resources.loadTexture(Render::assetPath("orangeshoot.png"));
    m_Audio.loadSound(Render::assetPath("sounds/enemy_shoot.wav"));
}

/**
 * Sets the targetted framerate
 * for the game. The actual framerate
 * will always be equal to (if everything
 * runs smoothly) or under (if the game
 * is hanging) the specified target framerate.
 */
void RType::Game::Game::setFPS(double targetFPS) noexcept
{
    using seconds = std::chrono::duration<double>;

    m_TargetDuration  = seconds(1.0 / targetFPS);
    m_FixedUpdateRate = seconds(FIXED_UPDATES_PER_FPS / targetFPS);
}

/**
 * Runs the game until the
 * opened window is instructed
 * to close.
 */
void RType::Game::Game::run()
{
    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    Wrecks::DeltaTime prevDelta = m_TargetDuration;

    while (!m_Window.shouldClose()) {
        update(prevDelta);
        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();

        // Reach target duration (i.e. target framerate)
        Wrecks::DeltaTime delta = frameEnd - frameStart;
        Wrecks::DeltaTime sleepTime = std::max(m_TargetDuration - delta, Wrecks::DeltaTime(0.0));
        std::this_thread::sleep_for(sleepTime);

        // Update for next frame
        frameStart = frameEnd + std::chrono::duration_cast<std::chrono::nanoseconds>(sleepTime);
        prevDelta = delta + sleepTime;
    }
}

/**
 * Runs a single game update.
 *
 * Game updates are divided into
 * 3 steps, in the following order:
 *
 * - fixed updates (if any)
 * - frame update
 * - render update
 *
 * Multiple or no fixed updates can
 * occur in a single frame to ensure
 * that for a given time period n,
 * fixed updates are called n*R times
 * (where R is the fixed update rate).
 *
 * As such, if the game is laggy, multiple
 * fixed updates may have to occur in a single
 * call to update() (so as to catch up to the
 * updates that should've occurred in the elapsed
 * time). On the other hand, if the game is running
 * very smoothly, there may be no fixed updates.
 */
void RType::Game::Game::update(Wrecks::DeltaTime dt)
{
    m_AccumulatedDelta += dt;
    while (m_AccumulatedDelta >= m_FixedUpdateRate) {
        m_Engine.fixedUpdate(m_FixedUpdateRate);
        m_AccumulatedDelta -= m_FixedUpdateRate;
    }

    m_Engine.update(dt);

    m_Window.beginFrame(Render::Colors::BLACK);
    m_Engine.render(dt);
    m_Window.endFrame();

    // FIXME: remove (shouldn't be here)
    // m_Audio.update();
    // m_Engine.get<Systems::Dead>().flush();
    // m_Engine.get<Systems::Shoot>().flush();
}
