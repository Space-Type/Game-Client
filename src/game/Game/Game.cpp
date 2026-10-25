/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Game class
*/

#include "Game.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <thread>


RType::Game::Game::Game()
    : m_Window({.title = "Spaceship demo", .width = 800, .height = 450, .virtualWidth = 800, .virtualHeight = 450})
    , m_Resources(m_Window)
    , m_Audio()
    , m_Renderer(m_Resources)
    , m_Input(m_Window)
    , m_Controls(defaultControls())
    , m_Engine()
    , m_FrameDuration(std::chrono::duration<double>(1.0 / 120))  // 120 FPS
    , m_DeltaTime(0.0f)
{
    m_Engine.get<Systems::Render>().setRenderer(m_Renderer);
    m_Engine.get<Systems::Controllable>().setInput(m_Input);
    m_Engine.get<Systems::Controllable>().setControls(m_Controls);
    m_Engine.get<Systems::Controllable>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::RandomMovement>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::RandomMovement>().setWindow(m_Window);
    m_Engine.get<Systems::RandomMovement>().setResources(m_Resources);
    m_Engine.get<Systems::Shoot>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::Shoot>().setResources(m_Resources);
    m_Engine.get<Systems::Shoot>().setAudio(m_Audio);
    m_Engine.get<Systems::Collision>().setAudio(m_Audio);
    m_Engine.get<Systems::Projectile>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::Projectile>().setWindow(m_Window);
    m_Engine.get<Systems::Starfield>().setDeltaTime(m_DeltaTime);
    m_Engine.get<Systems::Starfield>().setWindow(m_Window);
    m_Engine.get<Systems::StarRender>().setRenderer(m_Renderer);

    createStarfield();

    auto playerTexture = m_Resources.loadTexture(Render::assetPath("spaceship.png"));
    auto playerBulletTexture = m_Resources.loadTexture(Render::assetPath("blueshoot.png"));
    auto playerShootSound = m_Audio.loadSound(Render::assetPath("sounds/playershoot.wav"));
    auto enemyHitSound = m_Audio.loadSound(Render::assetPath("sounds/enemy_hit.wav"));
    const Render::Vec2D playerSize = m_Resources.textureSize(playerTexture);
    int playerCount = 1;

    for (int i = 0; i < playerCount; ++i) {
        auto player = m_Engine.createEntity();

        m_Engine.addComponent(player, Protecs::Components::Transform{ { 0.0f, 0.0f }, 0.f, 1.0f });
        m_Engine.addComponent(player, Protecs::Components::Velocity{});
        m_Engine.addComponent(player, Components::Controllable{});
        m_Engine.addComponent(player, Components::Sprite{ .texture = playerTexture });
        m_Engine.addComponent(player, Components::Hitbox{ .width = playerSize.x, .height = playerSize.y, .team = Components::Team::Player });
        m_Engine.addComponent(player, Components::Health{ .hp = 3 });
        m_Engine.addComponent(player, Components::Shooter{
            .bulletTexture = playerBulletTexture,
            .shootSound = playerShootSound,
            .hitSound = enemyHitSound,
        });
    }

    auto enemyTexture = m_Resources.loadTexture(Render::assetPath("ennemi.png"));
    auto enemyBulletTexture = m_Resources.loadTexture(Render::assetPath("orangeshoot.png"));
    auto enemyShootSound = m_Audio.loadSound(Render::assetPath("sounds/enemy_shoot.wav"));
    const Render::Vec2D enemySize = m_Resources.textureSize(enemyTexture);
    int enemyCount = 5;

    for (int i = 0; i < enemyCount; ++i) {
        auto enemy = m_Engine.createEntity();

        m_Engine.addComponent(enemy, Protecs::Components::Transform{ { m_Window.virtualSize().x + i * 60.0f, 50.0f + i * 70.0f }, 0.0f, 1.0f });
        m_Engine.addComponent(enemy, Components::HasRandomMovement{ .directionY = 0, .timer = i * 0.2f });
        m_Engine.addComponent(enemy, Components::Sprite{ .texture = enemyTexture });
        m_Engine.addComponent(enemy, Components::Hitbox{ .width = enemySize.x, .height = enemySize.y, .team = Components::Team::Enemy });
        m_Engine.addComponent(enemy, Components::Health{ .hp = 1 });
        m_Engine.addComponent(enemy, Components::Shooter{
            .bulletTexture = enemyBulletTexture,
            .shootSound = enemyShootSound,
            .cooldown = 1.5f,
            .timer = 1.0f + i * 0.3f,  // staggered so enemies don't all fire at once
            .bulletSpeed = -300.0f,
            .autoFire = true,
        });
    }
}

void RType::Game::Game::createStarfield()
{
    struct Layer {
        int count;
        Components::Star star;
    };

    // Far layers: more stars, smaller, dimmer and slower (parallax)
    static constexpr std::array<Layer, 3> LAYERS = {{
        { .count = 70, .star = { .speed = 20.0f, .size = 1.0f, .color = {90, 90, 110, 255} } },
        { .count = 35, .star = { .speed = 45.0f, .size = 2.0f, .color = {160, 160, 180, 255} } },
        { .count = 15, .star = { .speed = 90.0f, .size = 3.0f, .color = {255, 255, 255, 255} } },
    }};

    std::mt19937 random{std::random_device{}()};
    const Render::Vec2D screen = m_Window.virtualSize();
    std::uniform_real_distribution<float> randomX(0.0f, screen.x);
    std::uniform_real_distribution<float> randomY(0.0f, screen.y);

    for (const auto& layer : LAYERS) {
        for (int i = 0; i < layer.count; ++i) {
            auto star = m_Engine.createEntity();

            m_Engine.addComponent(star, Protecs::Components::Transform{ { randomX(random), randomY(random) }, 0.0f, 1.f });
            m_Engine.addComponent(star, Components::Star(layer.star));
        }
    }
}

void RType::Game::Game::run()
{
    std::chrono::time_point frameStart = std::chrono::steady_clock::now();
    std::chrono::time_point lastFrame = frameStart;

    while (!m_Window.shouldClose()) {
        std::chrono::time_point now = std::chrono::steady_clock::now();
        // Capped so a long stall (window drag, breakpoint) doesn't teleport entities
        m_DeltaTime = std::min(std::chrono::duration<float>(now - lastFrame).count(), 0.1f);
        lastFrame = now;

        m_Audio.update();
        m_Engine.update();
        m_Engine.get<Systems::Dead>().flush();
        m_Engine.get<Systems::Shoot>().flush();
        m_Window.beginFrame(Render::Colors::BLACK);
        m_Engine.render();
        m_Window.endFrame();

        std::chrono::time_point frameEnd = std::chrono::steady_clock::now();

        Duration delta = frameEnd - frameStart;
        Duration sleepTime = std::max(m_FrameDuration - delta, Duration(0));
        std::this_thread::sleep_for(sleepTime);
        frameStart = frameEnd + std::chrono::duration_cast<std::chrono::nanoseconds>(sleepTime);
    }

}
