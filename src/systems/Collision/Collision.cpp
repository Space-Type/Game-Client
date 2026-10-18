/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Collision System
*/

#include "Collision.hpp"
#include <rtype/game.hpp>


RType::Systems::Collision::Collision() noexcept
    : m_Engine(nullptr)
    , m_Audio(nullptr)
{

}

void RType::Systems::Collision::operator()(Wrecks::ECS::Entity bullet)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    if (Dead::isDead(engine, bullet))
        return;

    const auto& bulletTransform = engine.get<Components::Transform>()[bullet];
    const auto& bulletHitbox = engine.get<Components::Hitbox>()[bullet];
    const auto& projectile = engine.get<Components::Projectile>()[bullet];

    const Wrecks::Signature targetSignature = Wrecks::Signature()
        .set(Components::Manager::index<Components::Transform>())
        .set(Components::Manager::index<Components::Hitbox>());

    for (auto [target, health] : engine.get<Components::Health>()) {
        if ((engine.entitySignature(target) & targetSignature) != targetSignature || Dead::isDead(engine, target))
            continue;

        const auto& targetTransform = engine.get<Components::Transform>()[target];
        const auto& targetHitbox = engine.get<Components::Hitbox>()[target];

        if (targetHitbox.team == bulletHitbox.team)
            continue;

        // AABB overlap test
        const bool overlaps = bulletTransform.x < targetTransform.x + targetHitbox.width
                           && bulletTransform.x + bulletHitbox.width > targetTransform.x
                           && bulletTransform.y < targetTransform.y + targetHitbox.height
                           && bulletTransform.y + bulletHitbox.height > targetTransform.y;
        if (!overlaps)
            continue;

        if (m_Audio)
            m_Audio->play(projectile.hitSound);
        health.hp -= projectile.damage;
        if (health.hp <= 0)
            Dead::kill(engine, target);
        Dead::kill(engine, bullet);
        return;
    }
}
