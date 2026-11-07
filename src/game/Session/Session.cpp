/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Session class
*/

#include "Session.hpp"
#include "protecs/common.hpp"
#include <rtype/errors.hpp>
#include <rtype/game.hpp>
#include "../SpriteSheets/SpriteSheets.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>


RType::Game::Session::Session(Render::Resources& resources, const Render::Renderer& renderer, Render::Audio& audio,
                              const Render::Input& input, const Controls& controls, const DisplayOptions& display)
    : m_Resources(resources)
    , m_Renderer(renderer)
    , m_Audio(audio)
    , m_Starfield({ .x = static_cast<float>(Game::Game::VIRTUAL_WIDTH), .y = static_cast<float>(Game::Game::VIRTUAL_HEIGHT) })
    , m_PlayerShotSound(audio.loadSound(Render::assetPath("sounds/playershoot.wav")))
    , m_EnemyShotSound(audio.loadSound(Render::assetPath("sounds/enemy_shoot.wav")))
    , m_ExplosionSound(audio.loadSound(Render::assetPath("sounds/enemy_hit.wav")))
    , m_Effects()
    , m_Engine()
    , m_EntityMap()
    , m_SimulationTick(0)
    , m_ServerTickDelay(10)
    , m_AccumulatedDelta()
    , m_Ended(false)
    , m_EndReason()
    , m_Client()
{
    m_Engine.get<Systems::RenderSprite>().setRenderer(renderer);
    m_Engine.get<Systems::RenderOutline>().setRenderer(renderer);
    m_Engine.get<Systems::RenderOutline>().setResources(resources);
    m_Engine.get<Systems::RenderOutline>().setOptions(display);
    m_Engine.get<Systems::RenderName>().setRenderer(renderer);
    m_Engine.get<Systems::RenderName>().setResources(resources);
    m_Engine.get<Systems::PlayerInput>().setInput(input);
    m_Engine.get<Systems::PlayerInput>().setControls(controls);
    m_Engine.get<Systems::PlayerInput>().setInputQueuer([&](Protecs::Networking::Packets::PlayerInput input){
        input.simulationTick = m_SimulationTick + m_ServerTickDelay;
        m_Client.send(input);
    });
}

RType::Game::Session::~Session()
{
    // A failed goodbye must not get in the way of ending the session
    try {
        m_Client.disconnect();
    } catch (...) {
    }
}

void RType::Game::Session::beginConnect(const Protecs::Networking::Address& server, std::string_view name,
                                        std::chrono::milliseconds timeout)
{
    m_Client.beginConnect(server, name, timeout);
}

bool RType::Game::Session::pollConnect()
{
    return m_Client.pollConnect();
}

/**
 * Runs a single session update.
 *
 * Session updates are divided into
 * 3 steps, in the following order:
 *
 * - network events
 * - fixed updates (if any)
 * - frame update
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
void RType::Game::Session::update(Wrecks::DeltaTime dt)
{
    handleEvents(0);

    m_AccumulatedDelta += dt;
    while (m_AccumulatedDelta >= Protecs::FIXED_UPDATE_RATE) {
        handleEvents(m_SimulationTick);
        m_Engine.fixedUpdate(Protecs::FIXED_UPDATE_RATE);
        m_SimulationTick++;
        m_AccumulatedDelta -= Protecs::FIXED_UPDATE_RATE;

        if (m_SimulationTick % 1000 == 0)
            m_Client.send(Protecs::Networking::Packets::TickSync{ .clientTick = m_SimulationTick });

        if (m_SimulationTick % 1'000'000 == 0)
            m_Client.send(Protecs::Networking::Packets::EntityList{});
    }

    m_Engine.update(dt);
    m_Starfield.update(dt);
    removeFinishedEffects();
}

/**
 * Draws the session's entities.
 * Must be called between `Window::beginFrame()`
 * and `Window::endFrame()`.
 */
void RType::Game::Session::render(Wrecks::DeltaTime dt)
{
    m_Starfield.draw(m_Renderer);  // behind every entity
    m_Engine.render(dt);
}

void RType::Game::Session::setControlsEnabled(bool enabled) noexcept
{
    m_Engine.get<Systems::PlayerInput>().setEnabled(enabled);
}

void RType::Game::Session::handleEvents(Protecs::Tick tick)
{
    auto event = m_Client.receive(tick);

    for (unsigned int evtCount = 0; event && evtCount < MAX_EVENTS_PER_UPDATE; evtCount++) {
        // A bad packet only drops itself, not the session
        try {
            std::visit(EventHandler{ *this }, *event);
        } catch (const std::exception& err) {
            std::cerr << "[network] dropped packet: " << err.what() << std::endl;
        }

        event = m_Client.receive(tick);
    }
}

/**
 * The colour of a player comes from its slot (PlayerStats) once
 * the server sends it. Until then, from its network id: the
 * same on every client.
 */
void RType::Game::Session::initPlayer(Wrecks::ECS::Entity entity, Protecs::NetworkEntity id)
{
    m_Engine.addComponent(entity, Components::Controllable{});
    m_Engine.addComponent(entity, playerSprite(m_Resources, static_cast<std::size_t>(id % PLAYER_COLOURS)));
}

/**
 * An explosion where a ship or an enemy was destroyed:
 * the visual cue of the explosion sound (accessibility).
 */
void RType::Game::Session::explode(Wrecks::ECS::Entity entity)
{
    using Protecs::Networking::Packets::EntityCreate;

    if (!m_Engine.hasComponent<Components::EntityKind>(entity) || !m_Engine.hasComponent<Protecs::Components::Transform>(entity))
        return;

    EntityCreate::EntityType type = m_Engine.get<Components::EntityKind>()[entity].type;

    if (type != EntityCreate::PLAYER && type != EntityCreate::ENEMY_PATA_PATA && type != EntityCreate::ENEMY_SCANT)
        return;

    auto effect = m_Engine.createEntity();

    m_Engine.addComponent(effect, Protecs::Components::Transform{ m_Engine.get<Protecs::Components::Transform>()[entity] });
    m_Engine.addComponent(effect, explosionSprite(m_Resources));
    m_Effects.push_back(effect);
    m_Audio.play(m_ExplosionSound);
}

void RType::Game::Session::removeFinishedEffects()
{
    std::erase_if(m_Effects, [&](Wrecks::ECS::Entity effect) {
        if (!Components::finished(m_Engine.get<Components::Sprite>()[effect]))
            return false;
        m_Engine.deleteEntity(effect);
        return true;
    });
}

void RType::Game::Session::initSprite(Wrecks::ECS::Entity entity, Protecs::Networking::Packets::EntityCreate::EntityType type)
{
    switch (type) {
        case Protecs::Networking::Packets::EntityCreate::ENEMY_PATA_PATA:
        case Protecs::Networking::Packets::EntityCreate::ENEMY_SCANT:
            m_Engine.addComponent(entity, enemySprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::PLAYER_MISSILE:
            m_Engine.addComponent(entity, playerMissileSprite(m_Resources));
            m_Audio.play(m_PlayerShotSound);
            return;
        case Protecs::Networking::Packets::EntityCreate::ENEMY_MISSILE:
            m_Engine.addComponent(entity, enemyMissileSprite(m_Resources));
            m_Audio.play(m_EnemyShotSound);
            return;
        default:
            return;
    }
}


////////////////////////////////////////////////////////////////
//                                                            //
//                       Network Events                       //
//                                                            //
////////////////////////////////////////////////////////////////

namespace Packets = Protecs::Networking::Packets;

void RType::Game::Session::EventHandler::operator()(const Packets::EntityComponentData& data)
{
    Wrecks::ECS::Entity localEntity;

    try {
        localEntity = session.m_EntityMap.toLocal(data.entity);
    } catch (const Errors::Error& err) {
        throw Errors::NetworkError("EntityComponentData: no such entity", err);
    }

    std::visit([&, localEntity](auto& data){
        using Component = std::decay_t<decltype(data)>;
        if (!session.m_Engine.hasComponent<Component>(localEntity))
            session.m_Engine.addComponent(localEntity, Component{});

        session.m_Engine.get<Component>()[localEntity] = std::move(data);

        if constexpr (std::is_same_v<Component, Protecs::Components::PlayerStats>)
            if (session.m_Engine.hasComponent<Components::Sprite>(localEntity))
                setPlayerColour(session.m_Engine.get<Components::Sprite>()[localEntity], session.m_Engine.get<Component>()[localEntity].slot);
    }, data.component);
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityCreate& data)
{
    // Already added, nothing to do
    if (session.m_EntityMap.containsNetwork(data.id))
        return;

    auto entity = session.m_Engine.createEntity();
    session.m_EntityMap.bind(data.id, entity);
    session.m_Engine.addComponent(entity, Components::EntityKind{ data.type });

    if (data.type == Packets::EntityCreate::PLAYER)
        session.initPlayer(entity, data.id);
    else
        session.initSprite(entity, data.type);
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityDelete& data)
{
    // Already deleted, nothing to do
    if (!session.m_EntityMap.containsNetwork(data.id))
        return;

    auto localEntity = session.m_EntityMap.toLocal(data.id);
    session.explode(localEntity);
    session.m_Engine.deleteEntity(localEntity);
    session.m_EntityMap.removeNetwork(data.id);
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityList& data)
{
    for (auto [netEntity, signature] : data.entities) {
    }

    for (auto [netEntity, signature] : data.entities) {
        Wrecks::ECS::Entity localEntity;

        if (session.m_EntityMap.containsNetwork(netEntity)) {
            localEntity = session.m_EntityMap.toLocal(netEntity);
        } else {
            localEntity = session.m_Engine.createEntity();
            session.m_EntityMap.bind(netEntity, localEntity);
        }

        // local components don't matter, only shared ones
        auto localSignature = session.m_Engine.entitySignature(localEntity) & Protecs::Components::Manager::mask();

        if (signature == localSignature)
            continue;

        session.m_Client.send(Packets::EntitySyncComponents{
            .simulationTick = session.m_SimulationTick + session.m_ServerTickDelay,
            .entity = netEntity,
            .signature = localSignature,
        });
    }
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityRemoveComponent& data)
{

    Wrecks::ECS::Entity localEntity;

    try {
        localEntity = session.m_EntityMap.toLocal(data.entity);
    } catch (const Errors::Error& err) {
        throw Errors::NetworkError("EntityRemoveComponent: no such entity", err);
    }

    try {
        Wrecks::Utils::visitType<Protecs::Components::Variant>([&](auto type){
            using Component = decltype(type)::type;

            if (!session.m_Engine.hasComponent<Component>(localEntity))
                return;

            session.m_Engine.removeComponent<Component>(localEntity);
        }, data.type);
    } catch (const std::out_of_range& err) {
        throw Errors::NetworkError("EntityRemoveComponent: unknown component type", err);
    }
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntitySyncComponents& data)
{

}

void RType::Game::Session::EventHandler::operator()(const Packets::Ping& data)
{
    // reply with a ping to tell the
    // server you're still alive
    session.m_Client.send(Packets::Ping{});
}

void RType::Game::Session::EventHandler::operator()(const Packets::PlayerId& data)
{

}

void RType::Game::Session::EventHandler::operator()(const Packets::PlayerInput& data)
{

}

void RType::Game::Session::EventHandler::operator()(const Packets::PlayerJoin& data)
{

}

void RType::Game::Session::EventHandler::operator()(const Packets::PlayerLeave& data)
{
    session.m_Ended = true;
    session.m_EndReason = "Disconnected by the server";

    // Already ended by the server: a failed goodbye changes nothing
    try {
        session.m_Client.disconnect();
    } catch (...) {
    }
}

void RType::Game::Session::EventHandler::operator()(const Packets::TickSync& data)
{
    session.m_ServerTickDelay = session.m_SimulationTick - data.clientTick + 5;
    session.m_SimulationTick = data.serverTick;
}
