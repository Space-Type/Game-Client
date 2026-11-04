#include "Controllable.hpp"
#include <rtype/game.hpp>
#include <rtype/render.hpp>

RType::Systems::Controllable::Controllable() noexcept
    : m_Engine(nullptr)
    , m_Input(nullptr)
    , m_Controls(nullptr)
    , m_Enabled(true)
{

}

void RType::Systems::Controllable::operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt)
{
    if (!m_Enabled)
        return;

    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& controllable = engine.get<Components::Controllable>()[entity];
    auto& velocity     = engine.get<Protecs::Components::Velocity>()[entity];

    #define isDown(action) m_Controls->isDown(Game::Action::action, *m_Input)
    Render::Vec2D input = {
        .x = static_cast<float>(controllable.acceleration * dt.count() * (isDown(Right) - isDown(Left))),
        .y = static_cast<float>(controllable.acceleration * dt.count() * (isDown(Down) - isDown(Up))),
    };
    #undef isDown

    input.normalize();
    velocity.position += input;
}
