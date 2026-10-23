#include "RandomMovement.hpp"
#include <rtype/game.hpp>
#include <cstdlib>

RType::Systems::RandomMovement::RandomMovement() noexcept
{

}

void RType::Systems::RandomMovement::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& randomMovement = engine.get<Components::HasRandomMovement>()[entity];
    auto& transforms = engine.get<Protecs::Components::Transform>()[entity];
    auto& sprite = engine.get<Components::Sprite>()[entity];

    const float dt = *m_DeltaTime;

    randomMovement.timer -= dt;

    if (randomMovement.timer <= 0) {
        randomMovement.directionY = std::rand() % 3 - 1;   // -1, 0 ou 1
        randomMovement.timer = 1.0f;                       // garde la direction 1 seconde
    }

    transforms.position.x -= randomMovement.speed * dt;
    transforms.position.y += randomMovement.directionY * randomMovement.speed * dt;

    const float height = m_Window->virtualSize().y - m_Resources->textureSize(sprite.texture).y;

    if (transforms.position.y < 0) {
        transforms.position.y = 0;
        randomMovement.directionY = 1;
    } else if (transforms.position.y > height) {
        transforms.position.y = height;
        randomMovement.directionY = -1;
    }

    if (transforms.position.x < 0)
        transforms.position.x = m_Window->virtualSize().x;

}

