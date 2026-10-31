/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Session class
*/

#include "Session.hpp"
#include <rtype/errors.hpp>
#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>


RType::Game::Session::Session(Render::Resources& resources, const Render::Renderer& renderer,
                              const Render::Input& input, const Controls& controls,
                              Wrecks::DeltaTime fixedUpdateRate)
    : m_Resources(resources)
    , m_Engine()
    , m_EntityMap()
    , m_FixedUpdateRate(fixedUpdateRate)
    , m_AccumulatedDelta()
    , m_Ended(false)
    , m_EndReason()
    , m_Client()
{
    m_Engine.get<Systems::RenderSprite>().setRenderer(renderer);
    m_Engine.get<Systems::Controllable>().setInput(input);
    m_Engine.get<Systems::Controllable>().setControls(controls);
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
 *
 * Both the frame delta and the accumulated
 * delta are capped by MAX_ACCUMULATED_DELTA
 * (at least one fixed update stays reachable).
 */
void RType::Game::Session::update(Wrecks::DeltaTime dt)
{
    handleEvents();

    dt = std::min(dt, MAX_ACCUMULATED_DELTA);
    m_AccumulatedDelta = std::min(m_AccumulatedDelta + dt, std::max(MAX_ACCUMULATED_DELTA, m_FixedUpdateRate));
    while (m_AccumulatedDelta >= m_FixedUpdateRate) {
        m_Engine.fixedUpdate(m_FixedUpdateRate);
        m_AccumulatedDelta -= m_FixedUpdateRate;
    }

    m_Engine.update(dt);

    // FIXME: remove (shouldn't be here)
    // m_Engine.get<Systems::Dead>().flush();
    // m_Engine.get<Systems::Shoot>().flush();
}

/**
 * Draws the session's entities.
 * Must be called between `Window::beginFrame()`
 * and `Window::endFrame()`.
 */
void RType::Game::Session::render(Wrecks::DeltaTime dt)
{
    m_Engine.render(dt);
}

void RType::Game::Session::setControlsEnabled(bool enabled) noexcept
{
    m_Engine.get<Systems::Controllable>().setEnabled(enabled);
}

void RType::Game::Session::handleEvents()
{
    auto event = m_Client.receive();

    while (event) {
        // A bad packet only drops itself, not the session
        try {
            std::visit(EventHandler{ *this }, *event);
        } catch (const std::exception& err) {
            std::cerr << "[network] dropped packet: " << err.what() << std::endl;
        }
        event = m_Client.receive();
    }
}

void RType::Game::Session::initPlayer(Wrecks::ECS::Entity entity)
{
    m_Engine.addComponent(entity, Components::Controllable{});
    m_Engine.addComponent(entity, Components::Sprite{ m_Resources.loadTexture(Render::assetPath("spaceship.png"))});
}

void RType::Game::Session::initEnemyPataPata(Wrecks::ECS::Entity entity)
{

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
    }, data.component);
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityCreate& data)
{
    // Already added, nothing to do
    if (session.m_EntityMap.containsNetwork(data.id))
        return;

    auto entity = session.m_Engine.createEntity();
    session.m_EntityMap.bind(data.id, entity);

    switch (data.type) {
        case Packets::EntityCreate::PLAYER: session.initPlayer(entity); return;
        case Packets::EntityCreate::ENEMY_PATA_PATA: session.initEnemyPataPata(entity); return;
        default: return;
    }
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityDelete& data)
{
    // Already deleted, nothing to do
    if (!session.m_EntityMap.containsNetwork(data.id))
        return;

    auto localEntity = session.m_EntityMap.toLocal(data.id);
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
