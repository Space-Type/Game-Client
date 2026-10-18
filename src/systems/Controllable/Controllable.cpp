#include "Controllable.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>

RType::Systems::Controllable::Controllable() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
    , m_DeltaTime(nullptr)
{

}

void RType::Systems::Controllable::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& controllable = engine.get<Components::Controllable>()[entity];
    auto& physicsState = engine.get<Components::Physics>()[entity];

    const float dt = *m_DeltaTime;

    physicsState.velocityX += controllable.acceleration * dt * (m_Input->isDown(RType::Render::Key::Right) - m_Input->isDown(RType::Render::Key::Left));
    physicsState.velocityY += controllable.acceleration * dt * (m_Input->isDown(RType::Render::Key::Down) - m_Input->isDown(RType::Render::Key::Up));

    if (engine.entitySignature(entity).test(Components::Manager::index<Components::Shooter>()))
        engine.get<Components::Shooter>()[entity].wantsToShoot = m_Input->isDown(RType::Render::Key::Space);
}