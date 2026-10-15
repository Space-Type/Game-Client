/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Physics System
*/

#include "Physics.hpp"
#include <rtype/game.hpp>


RType::Systems::Physics::Physics() noexcept
{

}

void RType::Systems::Physics::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);


}
