#include "Controllable.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>

RType::Systems::Controllable::Controllable() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
{

}

void RType::Systems::Controllable::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& controllable = engine.get<Components::Controllable>()[entity];
    auto& physicsState = engine.get<Components::Physics>()[entity];

    physicsState.velocityX += 3.f * (m_Input->isDown(RType::Render::Key::Right) - m_Input->isDown(RType::Render::Key::Left));
    physicsState.velocityY += 3.f * (m_Input->isDown(RType::Render::Key::Down) - m_Input->isDown(RType::Render::Key::Up));
}