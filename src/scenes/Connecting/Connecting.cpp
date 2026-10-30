/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Connecting scene
*/

#include "Connecting.hpp"
#include "../Text.hpp"
#include <exception>
#include <string>


RType::Scenes::Connecting::Connecting() noexcept
    : m_Started(false)
{

}

/**
 * The first update only lets one frame show the
 * message: the second one blocks until the
 * server answers (or the connection fails).
 */
auto RType::Scenes::Connecting::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (!m_Started) {
        m_Started = true;
        return std::nullopt;
    }

    try {
        ctx.session = std::make_unique<Game::Session>(ctx.resources, ctx.renderer, ctx.input,
                                                      ctx.controls, ctx.fixedUpdateRate);
        ctx.session->connect("Player");  // nickname placeholder until #50
    } catch (const std::exception& err) {
        ctx.message = std::string("Connection failed: ") + err.what();
        ctx.session.reset();
        return SceneId::GameOver;
    }
    return SceneId::InGame;
}

void RType::Scenes::Connecting::render(Context& ctx) const
{
    drawCenteredText(ctx.renderer, ctx.window, "Connecting...", 340.0F, TEXT_SIZE);
}
