#pragma once

#include <rtype/components.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class RandomMovement {
        void* m_Engine;

        public:
            RandomMovement() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::HasRandomMovement>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
