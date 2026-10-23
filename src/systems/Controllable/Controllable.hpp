
#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include "../../game/Controls/Controls.hpp"
#include <wrecks.hpp>


namespace RType::Systems {
    class Controllable {
        void* m_Engine;
        const RType::Render::Input* m_Input;
        const Game::Controls* m_Controls;
        const float* m_DeltaTime;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = { .stage = Wrecks::ECS::SystemAttributes::STATE };

            Controllable() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Velocity>())
                    .set(Components::Manager::index<Components::Controllable>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setDeltaTime(const float& deltaTime) noexcept { m_DeltaTime = &deltaTime; }
            inline void setInput(const RType::Render::Input& input) noexcept { m_Input = &input; }
            inline void setControls(const Game::Controls& controls) noexcept { m_Controls = &controls; }

            void operator()(Wrecks::ECS::Entity entity);
    };
}
