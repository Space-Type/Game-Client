/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** InGame scene
*/

#include "InGame.hpp"
#include "../Text.hpp"


auto RType::Scenes::InGame::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (ctx.input.isPressed(Render::Key::Escape) || ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::Start))
        return SceneId::Paused;
    return std::nullopt;
}

void RType::Scenes::InGame::render(Context&) const
{

}
