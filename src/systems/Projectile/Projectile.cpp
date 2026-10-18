/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Projectile System
*/

#include "Projectile.hpp"
#include <rtype/game.hpp>


RType::Systems::Projectile::Projectile() noexcept
    : m_Engine(nullptr)
    , m_Window(nullptr)
    , m_DeltaTime(nullptr)
{

}

void RType::Systems::Projectile::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& transform = engine.get<Components::Transform>()[entity];
    const auto& hitbox = engine.get<Components::Hitbox>()[entity];
    const auto& projectile = engine.get<Components::Projectile>()[entity];

    // Not handled by Physics: it would stick bullets to the window borders
    transform.x += projectile.velocityX * *m_DeltaTime;

    if (transform.x + hitbox.width < 0 || transform.x > m_Window->virtualSize().x)
        Dead::kill(engine, entity);
}
