#include "RandomMovement.hpp"
#include <rtype/game.hpp>
#include <random>

RType::Systems::RandomMovement::RandomMovement() noexcept
{

}

void RType::Systems::RandomMovement::operator()(Wrecks::ECS::Entity entity)
{
    Game::Engine& engine = *reinterpret_cast<Game::Engine*>(m_Engine);

    auto& RandomMovement = engine.get<Components::HasRandomMovement>()[entity];
    auto& transforms = engine.get<Components::Transform>()[entity];

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-1, 1);

    transforms.x -= 1;
    transforms.y = 11;
}
