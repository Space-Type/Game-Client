/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include "../Gamepads/Gamepads.hpp"
#include "../Preferences/Preferences.hpp"
#include <rtype/errors.hpp>
#include <algorithm>
#include <exception>
#include <optional>
#include <utility>
#include <chrono>
#include <string>
#include <thread>


RType::Game::Game::Game(ConnectionInfo connection)
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
    , m_MenuControls(UI::defaultMenuControls())
    , m_DisplayOptions()
    , m_Gamepad()
    , m_TargetDuration()
    , m_Engine()
    , m_Soundtrack(m_Engine.createEntity())
    , m_Session()
    , m_Context{
        .window = m_Window,
        .renderer = m_Renderer,
        .input = m_Input,
        .audio = m_Audio,
        .resources = m_Resources,
        .controls = m_Controls,
        .menuControls = m_MenuControls,
        .displayOptions = m_DisplayOptions,
        .gamepad = m_Gamepad,
        .session = m_Session,
        .message = {},
        .outcome = RType::Game::Outcome::Message,
        .score = 0,
        .connection = std::move(connection),
        .settingsReturn = Scenes::SceneId::Connect,
        .clickSound = {},
    }
    , m_Scenes()
{
    setFPS(120);

    m_Resources.loadTexture(Render::assetPath("spaceship.png"));
    m_Resources.loadTexture(Render::assetPath("blueshoot.png"));
    m_Audio.loadSound(Render::assetPath("sounds/playershoot.wav"));
    m_Audio.loadSound(Render::assetPath("sounds/enemy_hit.wav"));

    m_Resources.loadTexture(Render::assetPath("ennemi.png"));
    m_Resources.loadTexture(Render::assetPath("orangeshoot.png"));
    m_Audio.loadSound(Render::assetPath("sounds/enemy_shoot.wav"));

    m_Context.clickSound = m_Audio.loadSound(Render::assetPath("sounds/click.wav"));

    m_Engine.get<Systems::PlayMusic>().setAudio(m_Audio);
    m_Engine.addComponent(m_Soundtrack, Components::Music{ .track = std::string(Session::TITLE_MUSIC) });

    loadGamepadMappings(m_Input);
    applyPreferences(loadPreferences(currentPreferences(m_Audio, m_Window, m_Controls, m_DisplayOptions, m_Gamepad)),
                     m_Audio, m_Window, m_Controls, m_DisplayOptions, m_Gamepad);
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

    m_Session.reset();
}

/**
 * Runs a single game update:
 *
 * - session update (if in a game)
 * - scene update
 * - render: session, then scene on top
 * - scene change (if any was requested)
 *
 * A session ended by the server, or
 * failing on a network error, leads
 * to the GameOver scene, whatever the
 * active scene requested.
 */
void RType::Game::Game::update(Wrecks::DeltaTime dt)
{
    std::optional<std::string> endMessage;

    // Gamepads come and go: the chosen one if connected, else the first one found
    m_Controls.setGamepad(chooseGamepad(connectedGamepads(m_Input), m_Gamepad));
    m_MenuControls.setGamepad(m_Controls.gamepad());

    if (m_Session) {
        m_Session->setControlsEnabled(m_Scenes.current() == Scenes::SceneId::InGame);
        try {
            m_Session->update(dt);
            if (m_Session->ended()) {
                endMessage = m_Session->endReason();
                m_Context.outcome = m_Session->outcome();
                m_Context.score = m_Session->finalScore();
            }
        } catch (const std::exception& err) {
            endMessage = std::string("Network error: ") + err.what();
            m_Context.outcome = RType::Game::Outcome::Message;
        }
    }

    m_Scenes.update(m_Context, dt);

    // After the scene update: GameOver wins over its request
    if (endMessage) {
        m_Context.message = std::move(*endMessage);
        m_Session.reset();
        m_Scenes.request(Scenes::SceneId::GameOver);
    }

    m_Window.beginFrame(Render::Colors::BLACK);
    if (m_Session)
        m_Session->render(dt);
    m_Scenes.render(m_Context);
    m_Window.endFrame(LETTERBOX_COLOR);

    m_Scenes.applyPending();
    if (m_Scenes.quitRequested())
        m_Window.close();

    // The soundtrack entity: the title music in the menus, the stage's in a game (Systems::PlayMusic streams it)
    m_Engine.get<Components::Music>()[m_Soundtrack].track = m_Session ? m_Session->music() : std::string(Session::TITLE_MUSIC);
    m_Engine.update(dt);
}
