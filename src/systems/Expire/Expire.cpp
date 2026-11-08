/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Expire system
*/

#include "Expire.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::Expire::Expire() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::Expire::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& lifetime = engine.get<Components::Lifetime>()[entity];

    lifetime.remaining -= std::chrono::duration<float>(dt).count();
    if (lifetime.remaining <= 0.0F)
        engine.deleteEntity(entity);
}
