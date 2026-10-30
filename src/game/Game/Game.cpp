/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <rtype/errors.hpp>
#include <algorithm>
#include <chrono>
#include <stdexcept>
#include <thread>


RType::Game::Game::Game()
    : m_Window({
        .title = "R-Type",
        .width = VIRTUAL_WIDTH,
        .height = VIRTUAL_HEIGHT,
        .virtualWidth = VIRTUAL_WIDTH,
        .virtualHeight = VIRTUAL_HEIGHT,
    })
    , m_Resources(m_Window)
    , m_Audio()
    , m_Renderer(m_Resources)
    , m_Input(m_Window)
    , m_Controls(defaultControls())
    , m_TargetDuration()
    , m_FixedUpdateRate()
    , m_Session()
{
    setFPS(120);

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

    m_Session = std::make_unique<Session>(m_Resources, m_Renderer, m_Input, m_Controls, m_FixedUpdateRate);
    m_Session->connect("John Doe");  // TODO: make this the player's ign

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

    m_Session.reset();
}

/**
 * Runs a single game update: the session
 * update, then the session render.
 */
void RType::Game::Game::update(Wrecks::DeltaTime dt)
{
    m_Session->update(dt);

    m_Window.beginFrame(Render::Colors::BLACK);
    m_Session->render(dt);
    m_Window.endFrame();

    // FIXME: remove (shouldn't be here)
    // m_Audio.update();
}
