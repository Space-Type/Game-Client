/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Shoot System
*/

#include "Shoot.hpp"
#include <rtype/game.hpp>


RType::Systems::Shoot::Shoot() noexcept
    : m_Engine(nullptr)
    , m_Resources(nullptr)
    , m_Audio(nullptr)
    , m_DeltaTime(nullptr)
    , m_Pending()
{

}

void RType::Systems::Shoot::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& shooter = engine.get<Components::Shooter>()[entity];
    const auto& transform = engine.get<Components::Transform>()[entity];
    const auto& hitbox = engine.get<Components::Hitbox>()[entity];

    if (shooter.timer > 0)
        shooter.timer -= *m_DeltaTime;
    if (shooter.timer > 0 || !(shooter.wantsToShoot || shooter.autoFire))
        return;
    shooter.timer = shooter.cooldown;
    m_Audio->play(shooter.shotSound);

    const RType::Render::Vec2 size = m_Resources->textureSize(shooter.bulletTexture);

    // Spawns in front of the shooter, vertically centered
    const float x = shooter.bulletSpeed > 0 ? transform.x + hitbox.width : transform.x - size.x;
    const float y = transform.y + (hitbox.height - size.y) / 2;

    m_Pending.push_back({
        .transform = { .x = x, .y = y, .rotation = 0.0f, .scale = 1.0f },
        .hitbox = { .width = size.x, .height = size.y, .team = hitbox.team },
        .projectile = { .velocityX = shooter.bulletSpeed, .damage = shooter.damage },
        .texture = shooter.bulletTexture,
    });
}

void RType::Systems::Shoot::flush()
{
    if (m_Pending.empty())
        return;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    for (auto& request : m_Pending) {
        auto bullet = engine.createEntity();

        engine.addComponent(bullet, std::move(request.transform));
        engine.addComponent(bullet, std::move(request.hitbox));
        engine.addComponent(bullet, std::move(request.projectile));
        engine.addComponent(bullet, Components::Sprite{ .texture = request.texture });
    }
    m_Pending.clear();
}
