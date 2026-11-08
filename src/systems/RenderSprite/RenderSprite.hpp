/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** RenderSprite system
*/

#pragma once

#include <rtype/components.hpp>
#include <rtype/render.hpp>
#include <wrecks.hpp>


namespace RType::Systems {
    /**
     * Draws each sprite at its entity's position (top-left
     * corner), and plays its animation. A player ship with a
     * velocity tilts: the sheet's middle frame when still,
     * the ones around it when moving up or down.
     */
    class RenderSprite {
        void* m_Engine;
        const RType::Render::Renderer* m_Renderer;

        public:
            static constexpr Wrecks::ECS::SystemAttributes ATTRIBUTES = {
                .stage = Wrecks::ECS::SystemAttributes::RENDER,
                .order = 2,
            };

            // Frames of a player ship in its sheet row: tilted down, still, tilted up
            static constexpr std::size_t TILT_DOWN_FRAME = 1;
            static constexpr std::size_t STILL_FRAME = 2;
            static constexpr std::size_t TILT_UP_FRAME = 3;

            RenderSprite() noexcept;

            static constexpr Wrecks::Signature signature() noexcept
            {
                return Wrecks::Signature()
                    .set(Components::Manager::index<Protecs::Components::Transform>())
                    .set(Components::Manager::index<Components::Sprite>());
            }

            inline void setEngine(void* engine) noexcept { m_Engine = engine; }
            inline void setRenderer(const RType::Render::Renderer& renderer) noexcept { m_Renderer = &renderer; }

            void operator()(Wrecks::ECS::Entity entity, Wrecks::DeltaTime dt);
    };

    // Size of the sprite's frame (or whole texture) in texture pixels
    Render::Vec2D spriteSize(const Components::Sprite& sprite, const Render::Resources& resources) noexcept;

    // Frame of a player ship sheet row for a vertical velocity (world y goes down)
    std::size_t tiltFrame(float velocityY) noexcept;
}
