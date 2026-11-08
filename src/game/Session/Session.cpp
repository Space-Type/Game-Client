/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Session class
*/

#include "Session.hpp"
#include "protecs/common.hpp"
#include <chrono>
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
    , m_ChargeSound(audio.loadSound(Render::assetPath("sounds/charge.wav")))
    , m_BeamSound(audio.loadSound(Render::assetPath("sounds/beam.wav")))
    , m_LevelSound(audio.loadSound(Render::assetPath("sounds/level.wav")))
    , m_StageClearSound(audio.loadSound(Render::assetPath("sounds/stage_clear.wav")))
    , m_WarningSound(audio.loadSound(Render::assetPath("sounds/warning.wav")))
    , m_Effects()
    , m_Engine()
    , m_EntityMap()
    , m_SimulationTick(0)
    , m_ServerTickDelay(10)
    , m_AccumulatedDelta()
    , m_Ended(false)
    , m_EndReason()
    , m_GameOverIn()
    , m_LastSyncTime()
    , m_Client()
{
    m_Engine.get<Systems::RenderSprite>().setRenderer(renderer);
    m_Engine.get<Systems::RenderOutline>().setRenderer(renderer);
    m_Engine.get<Systems::RenderOutline>().setResources(resources);
    m_Engine.get<Systems::RenderOutline>().setOptions(display);
    m_Engine.get<Systems::RenderName>().setRenderer(renderer);
    m_Engine.get<Systems::RenderName>().setResources(resources);
    m_Engine.get<Systems::RenderStage>().setRenderer(renderer);
    m_Engine.get<Systems::RenderStage>().setResources(resources);
    m_Engine.get<Systems::RenderCharge>().setRenderer(renderer);
    m_Engine.get<Systems::RenderCharge>().setResources(resources);
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
    bool needsTickSync   = false;
    bool needsEntitySync = false;

    handleEvents(0);

    m_AccumulatedDelta += dt;
    while (m_AccumulatedDelta >= Protecs::FIXED_UPDATE_RATE) {
        handleEvents(m_SimulationTick);
        m_Engine.fixedUpdate(Protecs::FIXED_UPDATE_RATE);
        m_SimulationTick++;
        m_AccumulatedDelta -= Protecs::FIXED_UPDATE_RATE;

        if (m_SimulationTick % 100 == 0)
            needsTickSync = true;

        if (m_SimulationTick % 1'000'000 == 0)
            needsEntitySync = true;
    }

    m_Engine.update(dt);
    m_Clock += std::chrono::duration<float>(dt).count();
    m_Starfield.update(dt);
    removeFinishedEffects();
    updateChargeSound();

    if (m_GameOverIn) {
        *m_GameOverIn -= std::chrono::duration<float>(dt).count();
        if (*m_GameOverIn <= 0.0F)
            m_Ended = true;
    }

    if (needsTickSync) {
        auto timeNow = std::chrono::steady_clock::now();
        m_Client.send(Protecs::Networking::Packets::TickSync{ .clientTick = m_SimulationTick, .clientTime = timeNow });
        m_LastSyncTime = timeNow;
    }

    if (needsEntitySync) {
        m_Client.send(Protecs::Networking::Packets::EntityList{});
    }
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

auto RType::Game::Session::players() const -> std::vector<UI::HudLine>
{
    std::vector<std::pair<std::uint8_t, UI::HudLine>> players;
    std::optional<Wrecks::ECS::Entity> local;

    if (m_EntityMap.containsNetwork(m_Client.player()))
        local = m_EntityMap.toLocal(m_Client.player());

    for (auto [entity, stats] : m_Engine.get<Protecs::Components::PlayerStats>()) {
        std::string name = m_Engine.hasComponent<Protecs::Components::PlayerName>(entity)
                         ? std::string(m_Engine.get<Protecs::Components::PlayerName>()[entity].str()) : "Player";

        players.emplace_back(stats.slot, UI::HudLine{ .name = std::move(name), .lives = stats.lives, .score = stats.score, .local = entity == local });
    }
    std::ranges::sort(players, [](const auto& a, const auto& b) {
        return a.second.local != b.second.local ? a.second.local : a.first < b.first;
    });

    std::vector<UI::HudLine> lines;

    for (auto& [slot, line] : players)
        lines.push_back(std::move(line));
    return lines;
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
            std::cerr << err.what() << std::endl;
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
    if (id == m_Client.player())
        m_Engine.addComponent(entity, Components::LocalCharge{});
}

/**
 * The charge sound plays while the local player charges
 * (not for a quick tap) and stops when the trigger is released.
 */
void RType::Game::Session::updateChargeSound()
{
    bool charging = false;

    for (auto [entity, charge] : m_Engine.get<Components::LocalCharge>())
        charging = charging || charge.seconds >= Components::CHARGE_SOUND_DELAY;

    if (charging && !m_ChargeSoundPlaying)
        m_Audio.play(m_ChargeSound);
    else if (!charging && m_ChargeSoundPlaying)
        m_Audio.stop(m_ChargeSound);
    m_ChargeSoundPlaying = charging;
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
    const auto transform = m_Engine.get<Protecs::Components::Transform>()[entity];

    if (type == EntityCreate::ENEMY_DOBKERATOPS && m_Engine.hasComponent<Components::Sprite>(entity)) {
        // The boss blows up all over its body
        Render::Vec2D size = Systems::spriteSize(m_Engine.get<Components::Sprite>()[entity], m_Resources);

        for (float fx : { 0.2F, 0.5F, 0.8F })
            for (float fy : { 0.2F, 0.5F, 0.8F })
                explodeAt({ .position = { transform.position.x + size.x * transform.scale * fx, transform.position.y + size.y * transform.scale * fy },
                            .rotation = 0.0F, .scale = 1.5F });
        return;
    }
    if (type != EntityCreate::PLAYER && type != EntityCreate::ENEMY_PATA_PATA && type != EntityCreate::ENEMY_SCANT
        && type != EntityCreate::ENEMY_DOBKERATOPS_CORE && type != EntityCreate::ENEMY_DOBKERATOPS_TAIL)
        return;
    explodeAt(transform);
}

void RType::Game::Session::explodeAt(const Protecs::Components::Transform& where)
{
    auto effect = m_Engine.createEntity();

    m_Engine.addComponent(effect, Protecs::Components::Transform{ where });
    m_Engine.addComponent(effect, explosionSprite(m_Resources));
    m_Effects.push_back(effect);
    m_Audio.play(m_ExplosionSound);
}

void RType::Game::Session::announce(std::string text, float duration, bool scores, Render::SoundId sound)
{
    m_Announcement = Announcement{ .text = std::move(text), .start = m_Clock, .duration = duration, .scores = scores };
    m_Audio.play(sound);
}

// The boss core is dead: "STAGE CLEAR" with the scores, while the server prepares the next stage
void RType::Game::Session::clearStage()
{
    announce("STAGE CLEAR", STAGE_CLEAR_ANNOUNCEMENT, true, m_StageClearSound);
}

// The server cleared the last stage (Level::won): the session ends with the victory screen
void RType::Game::Session::win()
{
    for (const auto& line : players())
        if (line.local)
            m_FinalScore = line.score;
    m_Outcome = Outcome::Victory;
    m_GameOverIn = 0.0F;
}

// The boss gets angry: "WARNING" in big letters (the visual equivalent of the alarm) and its body flashes
void RType::Game::Session::enrage()
{
    announce("WARNING", WARNING_ANNOUNCEMENT, false, m_WarningSound);
    if (auto body = findKind(Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS)) {
        if (m_Engine.hasComponent<Components::Blink>(*body))
            m_Engine.get<Components::Blink>()[*body].remaining = WARNING_BLINK;
        else
            m_Engine.addComponent(*body, Components::Blink{ .remaining = WARNING_BLINK });
    }
}

auto RType::Game::Session::findKind(Protecs::Networking::Packets::EntityCreate::EntityType type) const -> std::optional<Wrecks::ECS::Entity>
{
    for (auto [entity, kind] : m_Engine.get<Components::EntityKind>())
        if (kind.type == type)
            return entity;
    return std::nullopt;
}

auto RType::Game::Session::announcement() const -> std::optional<std::pair<Announcement, float>>
{
    if (!m_Announcement || m_Clock - m_Announcement->start > m_Announcement->duration)
        return std::nullopt;
    return std::pair{ *m_Announcement, m_Clock - m_Announcement->start };
}

auto RType::Game::Session::bossHealth() const -> std::optional<Protecs::Components::Health>
{
    std::optional<Wrecks::ECS::Entity> core = findKind(Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS_CORE);

    if (!core || !m_Engine.hasComponent<Protecs::Components::Health>(*core))
        return std::nullopt;
    return m_Engine.get<Protecs::Components::Health>()[*core];
}

/**
 * The server sends the lives before moving the ship back
 * to its start: the explosion is where the ship was hit.
 */
void RType::Game::Session::loseLife(Wrecks::ECS::Entity player, std::uint8_t livesLeft)
{
    explode(player);
    if (livesLeft == 0) {
        bool isLocal = m_EntityMap.containsNetwork(m_Client.player()) && m_EntityMap.toLocal(m_Client.player()) == player;

        // The game goes on for the others: this client shows its game over, then leaves
        if (isLocal && !m_GameOverIn) {
            m_GameOverIn = GAME_OVER_DELAY;
            m_Outcome = Outcome::Defeat;
            m_FinalScore = m_Engine.get<Protecs::Components::PlayerStats>()[player].score;
        }
        return;
    }
    if (m_Engine.hasComponent<Components::Blink>(player))
        m_Engine.get<Components::Blink>()[player] = Components::Blink{};
    else
        m_Engine.addComponent(player, Components::Blink{});
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
        case Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS:
            m_Engine.addComponent(entity, bossSprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::ENEMY_DOBKERATOPS_TAIL:
            m_Engine.addComponent(entity, tailSprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_1:
        case Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_2:
        case Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_3:
        case Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_4:
        case Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_5:
            m_Engine.addComponent(entity, beamSprite(m_Resources, static_cast<std::uint8_t>(type - Protecs::Networking::Packets::EntityCreate::PLAYER_BEAM_1 + 1)));
            m_Audio.play(m_BeamSound);
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
        session.m_Client.send(Packets::EntityList{});
        throw Errors::NetworkError("EntityComponentData: no such entity", err);
    }

    Protecs::NetworkEntity networkEntity = data.entity;

    std::visit([&, localEntity](auto& data){
        using Component = std::decay_t<decltype(data)>;

        // Players that joined before us were never announced by an EntityCreate: their name tells
        if constexpr (std::is_same_v<Component, Protecs::Components::PlayerName>) {
            if (!session.m_Engine.hasComponent<Components::EntityKind>(localEntity)) {
                session.m_Engine.addComponent(localEntity, Components::EntityKind{ Packets::EntityCreate::PLAYER });
                session.initPlayer(localEntity, networkEntity);
            }
        }
        std::optional<std::uint8_t> livesBefore;
        std::optional<std::uint16_t> hpBefore;

        if constexpr (std::is_same_v<Component, Protecs::Components::PlayerStats>)
            if (session.m_Engine.hasComponent<Component>(localEntity))
                livesBefore = session.m_Engine.get<Component>()[localEntity].lives;
        if constexpr (std::is_same_v<Component, Protecs::Components::Health>)
            if (session.m_Engine.hasComponent<Component>(localEntity))
                hpBefore = session.m_Engine.get<Component>()[localEntity].hp;

        if (!session.m_Engine.hasComponent<Component>(localEntity))
            session.m_Engine.addComponent(localEntity, Component{});

        session.m_Engine.get<Component>()[localEntity] = std::move(data);

        if constexpr (std::is_same_v<Component, Protecs::Components::PlayerStats>) {
            const auto& stats = session.m_Engine.get<Component>()[localEntity];

            if (session.m_Engine.hasComponent<Components::Sprite>(localEntity))
                setPlayerColour(session.m_Engine.get<Components::Sprite>()[localEntity], stats.slot);
            if (livesBefore && stats.lives < *livesBefore)
                session.loseLife(localEntity, stats.lives);
        }

        // An enemy hit but not killed blinks briefly (the boss body when its core is hit)
        if constexpr (std::is_same_v<Component, Protecs::Components::Health>) {
            if (hpBefore && session.m_Engine.get<Component>()[localEntity].hp < *hpBefore) {
                Wrecks::ECS::Entity target = localEntity;
                bool isCore = session.m_Engine.hasComponent<Components::EntityKind>(localEntity)
                           && session.m_Engine.get<Components::EntityKind>()[localEntity].type == Packets::EntityCreate::ENEMY_DOBKERATOPS_CORE;

                if (auto body = isCore ? session.findKind(Packets::EntityCreate::ENEMY_DOBKERATOPS) : std::nullopt)
                    target = *body;
                if (session.m_Engine.hasComponent<Components::Blink>(target))
                    session.m_Engine.get<Components::Blink>()[target].remaining = HIT_BLINK;
                else
                    session.m_Engine.addComponent(target, Components::Blink{ .remaining = HIT_BLINK });
            }
        }

        // "LEVEL n" when a stage starts (the first Level received, then every change)
        if constexpr (std::is_same_v<Component, Protecs::Components::Level>) {
            const auto& level = session.m_Engine.get<Component>()[localEntity];

            if (!session.m_Stage || *session.m_Stage != level.stage) {
                session.m_Stage = level.stage;
                session.announce("LEVEL " + std::to_string(level.stage), LEVEL_ANNOUNCEMENT, false, session.m_LevelSound);
            }
            if (level.won && !session.m_GameOverIn)  // a player already out keeps its game over
                session.win();
        }

        // A boss getting angry (its phase goes up): "WARNING", an alarm, and its body flashes
        if constexpr (std::is_same_v<Component, Protecs::Components::BossPhase>) {
            std::uint8_t phase = session.m_Engine.get<Component>()[localEntity].phase;

            if (session.m_BossPhase && phase > *session.m_BossPhase)
                session.enrage();
            session.m_BossPhase = phase;
        }
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
    bool isCore = session.m_Engine.hasComponent<Components::EntityKind>(localEntity)
               && session.m_Engine.get<Components::EntityKind>()[localEntity].type == Packets::EntityCreate::ENEMY_DOBKERATOPS_CORE;

    if (!Components::isEliminated(session.m_Engine, localEntity))  // it already exploded
        session.explode(localEntity);
    if (isCore && !session.m_GameOverIn)
        session.clearStage();
    session.m_Engine.deleteEntity(localEntity);
    session.m_EntityMap.removeNetwork(data.id);
}

void RType::Game::Session::EventHandler::operator()(const Packets::EntityList& data)
{
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
    auto player = session.m_EntityMap.toLocal(session.m_Client.player());
    auto& input = session.m_Engine.get<Protecs::Components::PlayerInput>()[player];

    using Protecs::Networking::Packets::PlayerInput;

    static constexpr std::array<decltype(PlayerInput::input), 5> direction = {
        PlayerInput::UP, PlayerInput::DOWN,
        PlayerInput::LEFT, PlayerInput::RIGHT,
        PlayerInput::TRIGGER,
    };

    const std::array<bool, 5> isHoldingKey = {
        input.up, input.down,
        input.left, input.right,
        input.trigger,
    };

    for (std::uint8_t i = 0; i < direction.size(); i++) {
        session.m_Client.send(PlayerInput{
            .simulationTick = session.m_SimulationTick + session.m_ServerTickDelay,
            .sequence       = i,
            .input          = direction[i],
            .state          = isHoldingKey[i] ? PlayerInput::HOLD_DOWN : PlayerInput::RELEASE,
        });
    }
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
    if (session.m_LastSyncTime > data.clientTime)
        return;

    auto timeDelay = (std::chrono::steady_clock::now() - data.clientTime) / 2.0;
    session.m_ServerTickDelay = timeDelay / Protecs::FIXED_UPDATE_RATE + 2;
    session.m_SimulationTick = data.serverTick;
}
