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
    , m_TargetDuration()
    , m_FixedUpdateRate()
    , m_AccumulatedDelta()
    , m_EntityMap()
    , m_Client()
{
    setFPS(120);

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

    m_Client.connect("John Doe");  // TODO: make this the player's ign

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

    m_Client.disconnect();
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
    handleEvents();

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

void RType::Game::Game::handleEvents()
{
    auto event = m_Client.receive();

    while (event) {
        std::visit(EventHandler{ *this }, *event);
        event = m_Client.receive();
    }
}

void RType::Game::Game::initPlayer(Wrecks::ECS::Entity entity)
{
    m_Engine.addComponent(entity, Components::Controllable{});
    m_Engine.addComponent(entity, Components::Sprite{ m_Resources.loadTexture(Render::assetPath("spaceship.png"))});
}

void RType::Game::Game::initEnemyPataPata(Wrecks::ECS::Entity entity)
{

}


////////////////////////////////////////////////////////////////
//                                                            //
//                       Network Events                       //
//                                                            //
////////////////////////////////////////////////////////////////

namespace Packets = Protecs::Networking::Packets;

void RType::Game::Game::EventHandler::operator()(const Packets::EntityComponentData& data)
{
    auto localEntity = game.m_EntityMap.toLocal(data.entity);

    std::visit([&, localEntity](auto& data){
        using Component = std::decay_t<decltype(data)>;
        if (!game.m_Engine.hasComponent<Component>(localEntity))
            game.m_Engine.addComponent(localEntity, Component{});

        game.m_Engine.get<Component>()[localEntity] = std::move(data);
    }, data.component);
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityCreate& data)
{
    // Already added, nothing to do
    if (game.m_EntityMap.containsNetwork(data.id))
        return;

    auto entity = game.m_Engine.createEntity();
    game.m_EntityMap.bind(data.id, entity);

    switch (data.type) {
        case Packets::EntityCreate::PLAYER: game.initPlayer(entity); return;
        case Packets::EntityCreate::ENEMY_PATA_PATA: game.initEnemyPataPata(entity); return;
        default: return;
    }
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityDelete& data)
{
    // Already deleted, nothing to do
    if (!game.m_EntityMap.containsNetwork(data.id))
        return;

    auto localEntity = game.m_EntityMap.toLocal(data.id);
    game.m_Engine.deleteEntity(localEntity);
    game.m_EntityMap.removeNetwork(data.id);
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityList& data)
{
    for (auto& [netEntity, signature] : data.entities) {
        Wrecks::ECS::Entity localEntity;

        if (game.m_EntityMap.containsNetwork(netEntity)) {
            localEntity = game.m_EntityMap.toLocal(netEntity);
        } else {
            localEntity = game.m_Engine.createEntity();
            game.m_EntityMap.bind(netEntity, localEntity);
        }

        auto localSignature = game.m_Engine.entitySignature(localEntity);

        if (signature == localSignature)
            continue;

        game.m_Client.send(Packets::EntitySyncComponents{
            .entity = netEntity,
            .signature = localSignature,
        });
    }
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{
}

void RType::Game::Game::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{

}

void RType::Game::Game::EventHandler::operator()(const Packets::Ping& data)
{
    // reply with a ping to tell the
    // server you're still alive
    game.m_Client.send(Packets::Ping{});
}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerId& data)
{

}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerInput& data)
{

}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerJoin& data)
{

}

void RType::Game::Game::EventHandler::operator()(const Packets::PlayerLeave& data)
{
    game.m_Client.disconnect();
}
