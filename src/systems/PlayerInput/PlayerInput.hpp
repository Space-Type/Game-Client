/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** PlayerInput system
*/

#pragma once

#include "../../game/Controls/Controls.hpp"
#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    class PlayerInput {
        void* m_Engine;
        const RType::Render::Input* m_Input;
        const Game::Controls* m_Controls;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::UPDATE,
                .order = 0,
            };

            PlayerInput() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::PlayerInput>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setInput(const RType::Render::Input& input) noexcept { m_Input = &input; }
            inline void setControls(const Game::Controls& controls) noexcept { m_Controls = &controls; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime);
    };
}
