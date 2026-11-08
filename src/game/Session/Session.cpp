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
#include "../../ui/Theme.hpp"
#include <algorithm>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <type_traits>


RType::Game::Session::Session(Render::Resources& resources, const Render::Renderer& renderer, Render::Audio& audio,
                              const Render::Input& input, const Controls& controls, const DisplayOptions& display)
    : m_Resources(resources)
    , m_Audio(audio)
    , m_PlayerShotSound(audio.loadSound(Render::assetPath("sounds/playershoot.wav")))
    , m_EnemyShotSound(audio.loadSound(Render::assetPath("sounds/enemy_shoot.wav")))
    , m_ExplosionSound(audio.loadSound(Render::assetPath("sounds/enemy_hit.wav")))
    , m_ChargeSound(audio.loadSound(Render::assetPath("sounds/charge.wav")))
    , m_BeamSound(audio.loadSound(Render::assetPath("sounds/beam.wav")))
    , m_LevelSound(audio.loadSound(Render::assetPath("sounds/level.wav")))
    , m_StageClearSound(audio.loadSound(Render::assetPath("sounds/stage_clear.wav")))
    , m_WarningSound(audio.loadSound(Render::assetPath("sounds/warning.wav")))
    , m_LifeSound(audio.loadSound(Render::assetPath("sounds/life.wav")))
    , m_ForceSound(audio.loadSound(Render::assetPath("sounds/force.wav")))
    , m_LastComponentAt(std::chrono::steady_clock::now())
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
    constexpr Render::Vec2D SCREEN = { .x = static_cast<float>(Game::Game::VIRTUAL_WIDTH), .y = static_cast<float>(Game::Game::VIRTUAL_HEIGHT) };

    spawnStarfield(m_Engine, SCREEN);
    m_Engine.get<Systems::ScrollStars>().setArea(SCREEN);
    m_Engine.get<Systems::RenderStarfield>().setRenderer(renderer);
    m_Engine.get<Systems::PlayChargeSound>().setAudio(audio);
    m_Engine.get<Systems::ForgetStale>().setEntityMap(m_EntityMap);
    m_Engine.get<Systems::ForgetStale>().setLastComponentAt(m_LastComponentAt);
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
    m_Engine.get<Systems::RenderPopup>().setRenderer(renderer);
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
    m_Engine.render(dt);
}

auto RType::Game::Session::players() const -> std::vector<UI::HudLine>
{
    std::vector<std::pair<std::uint8_t, UI::HudLine>> players;
    std::optional<Wrecks::ECS::Entity> local = localPlayer();

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
    if (m_Client.isConnected() && id == m_Client.player()) {
        m_Engine.addComponent(entity, Components::LocalCharge{});
        m_Engine.addComponent(entity, Components::ChargeSound{ .sound = m_ChargeSound });
    }
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

    if (isBossBody(type) && m_Engine.hasComponent<Components::Sprite>(entity)) {
        // The boss blows up all over its body
        Render::Vec2D size = Systems::spriteSize(m_Engine.get<Components::Sprite>()[entity], m_Resources);

        for (float fx : { 0.2F, 0.5F, 0.8F })
            for (float fy : { 0.2F, 0.5F, 0.8F })
                explodeAt({ .position = { transform.position.x + size.x * transform.scale * fx, transform.position.y + size.y * transform.scale * fy },
                            .rotation = 0.0F, .scale = 1.5F });
        return;
    }
    if (type != EntityCreate::PLAYER && type != EntityCreate::ENEMY_PATA_PATA && type != EntityCreate::ENEMY_SCANT
        && !bossOf(type) && type != EntityCreate::ENEMY_DOBKERATOPS_TAIL && type != EntityCreate::ENEMY_POW_ARMOR)
        return;
    explodeAt(transform);
}

void RType::Game::Session::explodeAt(const Protecs::Components::Transform& where)
{
    auto effect = m_Engine.createEntity();

    Components::Sprite sprite = explosionSprite(m_Resources);

    m_Engine.addComponent(effect, Protecs::Components::Transform{ where });
    m_Engine.addComponent(effect, Components::Lifetime{ .remaining = Components::animationDuration(sprite) });
    m_Engine.addComponent(effect, std::move(sprite));
    m_Audio.play(m_ExplosionSound);
}

// One announcement at a time: a new one replaces the one on screen
void RType::Game::Session::announce(std::string text, float duration, bool scores, Render::SoundId sound)
{
    std::vector<Wrecks::ECS::Entity> shown;

    for (auto [entity, announcement] : m_Engine.get<Components::Announcement>())
        shown.push_back(entity);
    for (auto entity : shown)
        m_Engine.deleteEntity(entity);

    auto entity = m_Engine.createEntity();

    m_Engine.addComponent(entity, Components::Announcement{ .text = std::move(text), .duration = duration, .scores = scores });
    m_Engine.addComponent(entity, Components::Lifetime{ .remaining = duration });
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
    auto boss = findBoss();

    announce("WARNING", WARNING_ANNOUNCEMENT, false, m_WarningSound);
    if (auto body = boss ? findKind(boss->second->body) : std::nullopt) {
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
    for (auto [entity, announcement] : m_Engine.get<Components::Announcement>())
        if (m_Engine.hasComponent<Components::Lifetime>(entity))
            return std::pair{ announcement, Components::shownFor(announcement, m_Engine.get<Components::Lifetime>()[entity]) };
    return std::nullopt;
}

// The boss core in the game, and which boss it is
auto RType::Game::Session::findBoss() const -> std::optional<std::pair<Wrecks::ECS::Entity, const BossKinds*>>
{
    for (auto [entity, kind] : m_Engine.get<Components::EntityKind>())
        if (const BossKinds* boss = bossOf(kind.type))
            return std::pair{ entity, boss };
    return std::nullopt;
}

auto RType::Game::Session::bossBar() const -> std::optional<BossBar>
{
    auto boss = findBoss();

    if (!boss || !m_Engine.hasComponent<Protecs::Components::Health>(boss->first))
        return std::nullopt;
    return BossBar{ .name = boss->second->name, .health = m_Engine.get<Protecs::Components::Health>()[boss->first] };
}

/**
 * The ship of this client, once the server has accepted it and
 * created it. Nothing while connecting (the session is updated
 * before the join answer is read) or after the session ended.
 */
auto RType::Game::Session::localPlayer() const -> std::optional<Wrecks::ECS::Entity>
{
    if (!m_Client.isConnected() || !m_EntityMap.containsNetwork(m_Client.player()))
        return std::nullopt;
    return m_EntityMap.toLocal(m_Client.player());
}

/**
 * The server sends the lives before moving the ship back
 * to its start: the explosion is where the ship was hit.
 */
void RType::Game::Session::loseLife(Wrecks::ECS::Entity player, std::uint8_t livesLeft)
{
    explode(player);
    if (livesLeft == 0) {
        bool isLocal = localPlayer() == player;

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

/**
 * A bonus life taken: a sound, and "+1 LIFE" rising over
 * the ship so it is seen without the sound (accessibility).
 */
void RType::Game::Session::gainLife(Wrecks::ECS::Entity player)
{
    m_Audio.play(m_LifeSound);
    if (!m_Engine.hasComponent<Protecs::Components::Transform>(player))
        return;

    auto popup = m_Engine.createEntity();
    auto where = m_Engine.get<Protecs::Components::Transform>()[player];

    where.position.x += Protecs::PLAYER_WIDTH / 2.0F;  // above the middle of the ship
    m_Engine.addComponent(popup, Protecs::Components::Transform{ where });
    m_Engine.addComponent(popup, Components::Popup{ .text = "+1 LIFE", .color = UI::Theme::HEART_HIGH, .duration = LIFE_POPUP });
    m_Engine.addComponent(popup, Components::Lifetime{ .remaining = LIFE_POPUP });
}

/**
 * A Force appears when a player takes it, and again one
 * level up: a sound, and "FORCE" (with its level) over it.
 */
void RType::Game::Session::takeForce(Wrecks::ECS::Entity force, std::uint8_t level)
{
    auto popup = m_Engine.createEntity();

    m_Engine.addComponent(force, forceSprite(m_Resources, level));
    m_Audio.play(m_ForceSound);
    m_Engine.addComponent(popup, Components::Popup{
        .text = level > 1 ? "FORCE LV" + std::to_string(level) : "FORCE",
        .color = FORCE_POPUP_COLOR,
        .duration = FORCE_POPUP,
        .anchor = force,
    });
    m_Engine.addComponent(popup, Components::Lifetime{ .remaining = FORCE_POPUP });
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
        case Protecs::Networking::Packets::EntityCreate::PICKUP_LIFE:
            m_Engine.addComponent(entity, lifePickupSprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::ENEMY_POW_ARMOR:
            m_Engine.addComponent(entity, powArmorSprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::PICKUP_FORCE:
            m_Engine.addComponent(entity, forcePickupSprite(m_Resources));
            return;
        case Protecs::Networking::Packets::EntityCreate::FORCE_1:
        case Protecs::Networking::Packets::EntityCreate::FORCE_2:
        case Protecs::Networking::Packets::EntityCreate::FORCE_3:
            takeForce(entity, static_cast<std::uint8_t>(type - Protecs::Networking::Packets::EntityCreate::FORCE_1 + 1));
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

        // Any component counts: the level entity, for one, has no Transform
        session.m_LastComponentAt = std::chrono::steady_clock::now();
        if (session.m_Engine.hasComponent<Components::Freshness>(localEntity))
            session.m_Engine.get<Components::Freshness>()[localEntity].sinceUpdate = 0.0F;

        if constexpr (std::is_same_v<Component, Protecs::Components::PlayerStats>) {
            const auto& stats = session.m_Engine.get<Component>()[localEntity];

            if (session.m_Engine.hasComponent<Components::Sprite>(localEntity))
                setPlayerColour(session.m_Engine.get<Components::Sprite>()[localEntity], stats.slot);
            if (livesBefore && stats.lives < *livesBefore)
                session.loseLife(localEntity, stats.lives);
            else if (livesBefore && stats.lives > *livesBefore)
                session.gainLife(localEntity);
        }

        // An enemy hit but not killed blinks briefly (the boss body when its core is hit)
        if constexpr (std::is_same_v<Component, Protecs::Components::Health>) {
            if (hpBefore && session.m_Engine.get<Component>()[localEntity].hp < *hpBefore) {
                Wrecks::ECS::Entity target = localEntity;
                const BossKinds* boss = session.m_Engine.hasComponent<Components::EntityKind>(localEntity)
                                      ? bossOf(session.m_Engine.get<Components::EntityKind>()[localEntity].type) : nullptr;

                if (auto body = boss ? session.findKind(boss->body) : std::nullopt)
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
    session.m_Engine.addComponent(entity, Components::Freshness{});
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
               && bossOf(session.m_Engine.get<Components::EntityKind>()[localEntity].type);

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
            session.m_Engine.addComponent(localEntity, Components::Freshness{});
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
    // The ship may not be known yet (its EntityCreate was lost): the next request comes in 0.5 s
    auto local = session.localPlayer();

    if (!local)
        return;

    auto player = *local;

    // The server sends PlayerInput once, when the player joins: if that packet was lost,
    // start from "nothing held" (the PlayerInput system needs the component to send inputs)
    if (!session.m_Engine.hasComponent<Protecs::Components::PlayerInput>(player))
        session.m_Engine.addComponent(player, Protecs::Components::PlayerInput{});

    auto& input = session.m_Engine.get<Protecs::Components::PlayerInput>()[player];

    using Protecs::Networking::Packets::PlayerInput;

    static constexpr std::array<decltype(PlayerInput::input), 6> direction = {
        PlayerInput::UP, PlayerInput::DOWN,
        PlayerInput::LEFT, PlayerInput::RIGHT,
        PlayerInput::TRIGGER, PlayerInput::FORCE,
    };

    const std::array<bool, 6> isHoldingKey = {
        input.up, input.down,
        input.left, input.right,
        input.trigger, input.force,
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
