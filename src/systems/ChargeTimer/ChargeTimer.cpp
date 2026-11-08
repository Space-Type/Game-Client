/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** ChargeTimer system
*/

#include "ChargeTimer.hpp"
#include <rtype/game.hpp>
#include <chrono>


RType::Systems::ChargeTimer::ChargeTimer() noexcept
    : m_Engine(nullptr)
{

}

void RType::Systems::ChargeTimer::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& charge = engine.get<Components::LocalCharge>()[entity];

    if (engine.get<Protecs::Components::PlayerInput>()[entity].trigger)
        charge.seconds += std::chrono::duration<float>(dt).count();
    else
        charge.seconds = 0.0F;
}
