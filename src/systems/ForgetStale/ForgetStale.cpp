/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ForgetStale system
*/

#include "ForgetStale.hpp"
#include <rtype/game.hpp>


RType::Systems::ForgetStale::ForgetStale() noexcept
    : m_Engine(nullptr)
    , m_EntityMap(nullptr)
    , m_LastComponentAt(nullptr)
{

}

void RType::Systems::ForgetStale::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& freshness = engine.get<Components::Freshness>()[entity];

    freshness.sinceUpdate += std::chrono::duration<float>(dt).count();
    if (!m_EntityMap || !m_LastComponentAt || engine.hasComponent<Components::LocalCharge>(entity))
        return;

    float sinceAnyUpdate = std::chrono::duration<float>(std::chrono::steady_clock::now() - *m_LastComponentAt).count();

    if (!Components::isStale(freshness.sinceUpdate, sinceAnyUpdate))
        return;
    if (m_EntityMap->containsLocal(entity))
        m_EntityMap->removeNetwork(m_EntityMap->toNetwork(entity));
    engine.deleteEntity(entity);
}
