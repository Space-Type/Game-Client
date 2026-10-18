/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Dead System
*/

#include "Dead.hpp"
#include <rtype/game.hpp>


RType::Systems::Dead::Dead() noexcept
    : m_Engine(nullptr)
    , m_Pending()
{

}

void RType::Systems::Dead::operator()(Wrecks::ECS::Entity entity)
{
    m_Pending.push_back(entity);
}

void RType::Systems::Dead::flush()
{
    if (m_Pending.empty())
        return;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    for (Wrecks::ECS::Entity entity : m_Pending)
        engine.deleteEntity(entity);
    m_Pending.clear();
}
