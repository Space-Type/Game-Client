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


namespace Packets = Protecs::Networking::Packets;


RType::Systems::PlayerInput::PlayerInput() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
    , m_Controls(nullptr)
    , m_Enabled(true)
{

}

void RType::Systems::PlayerInput::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);
    auto& input = engine.get<Protecs::Components::PlayerInput>()[entity];

    if (!m_Enabled) {
        input = { false };
        return;
    }

    auto prevInput = input;

    #define isDown(action) m_Controls->isDown(Game::Action::action, *m_Input)
    input.up      = isDown(Up);
    input.down    = isDown(Down);
    input.left    = isDown(Left);
    input.right   = isDown(Right);
    input.trigger = isDown(Shoot);
    #undef isDown

    #define isHeldDown(key) (input.key ? Packets::PlayerInput::HOLD_DOWN : Packets::PlayerInput::RELEASE)
    #define queue(seq, keyUpper, key) m_QueuePlayerInput({ 0, seq, Packets::PlayerInput::keyUpper, isHeldDown(key) })
    if (input.up != prevInput.up)
        queue(0, UP, up);
    if (input.down != prevInput.down)
        queue(0, DOWN, down);
    if (input.left != prevInput.left)
        queue(0, LEFT, left);
    if (input.right != prevInput.right)
        queue(0, RIGHT, right);
    if (input.trigger != prevInput.trigger)
        queue(0, TRIGGER, trigger);
    #undef queue
    #undef isHeldDown
}
