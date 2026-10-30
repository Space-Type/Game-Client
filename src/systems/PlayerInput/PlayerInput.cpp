/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PlayerInput system
*/

#include "PlayerInput.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>


RType::Systems::PlayerInput::PlayerInput() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
    , m_Controls(nullptr)
{

}

void RType::Systems::PlayerInput::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& input = engine.get<Protecs::Components::PlayerInput>()[entity];

    #define isDown(action) m_Controls->isDown(Game::Action::action, *m_Input)
    input.up      = isDown(Up);
    input.down    = isDown(Down);
    input.left    = isDown(Left);
    input.right   = isDown(Right);
    input.trigger = isDown(Shoot);
    #undef isDown
}
