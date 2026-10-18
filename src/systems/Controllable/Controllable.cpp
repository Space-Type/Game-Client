#include "Controllable.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>

RType::Systems::Controllable::Controllable() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
    , m_Controls(nullptr)
    , m_DeltaTime(nullptr)
{

}

void RType::Systems::Controllable::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& controllable = engine.get<Components::Controllable>()[entity];
    auto& physicsState = engine.get<Components::Physics>()[entity];

    const float dt = *m_DeltaTime;

    const auto isDown = [&](Game::Action action) { return m_Controls->isDown(action, *m_Input); };

    physicsState.velocityX += controllable.acceleration * dt * (isDown(Game::Action::Right) - isDown(Game::Action::Left));
    physicsState.velocityY += controllable.acceleration * dt * (isDown(Game::Action::Down) - isDown(Game::Action::Up));

    if (engine.entitySignature(entity).test(Components::Manager::index<Components::Shooter>()))
        engine.get<Components::Shooter>()[entity].wantsToShoot = isDown(Game::Action::Shoot);
}