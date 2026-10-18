#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class RandomMovement {
        void* m_Engine;
        const RType::Render::Window* m_Window;
        const RType::Render::Resources* m_Resources;
        const float* m_DeltaTime;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::STATE };

            RandomMovement() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Components::Transform>())
                    .set(Components::Manager::index<Components::HasRandomMovement>())
                    .set(Components::Manager::index<Components::Sprite>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setDeltaTime(const float& deltaTime) noexcept { m_DeltaTime = &deltaTime; }
            inline void setWindow(const RType::Render::Window& window) noexcept { m_Window = &window; }
            inline void setResources(const RType::Render::Resources& resources) noexcept { m_Resources = &resources; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
