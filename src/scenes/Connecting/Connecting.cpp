/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Connecting scene
*/

#include "Connecting.hpp"
#include "../Text.hpp"
#include <rtype/errors.hpp>
#include <exception>
#include <memory>
#include <string>
#include <string_view>


namespace {
    constexpr float MESSAGE_Y = 300.0F;
    constexpr float HINT_Y = 400.0F;

    std::optional<RType::Scenes::SceneId> fail(RType::Scenes::Context& ctx, std::string_view reason)
    {
        ctx.message = "Connection failed: " + std::string(reason);
        ctx.session.reset();
        return RType::Scenes::SceneId::Connect;
    }
}


RType::Scenes::Connecting::Connecting() noexcept
    : m_Started(false)
{

}

auto RType::Scenes::Connecting::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    try {
        if (!m_Started) {
            m_Started = true;
            ctx.message.clear();
            ctx.session = std::make_unique<Game::Session>(ctx.resources, ctx.renderer, ctx.input,
                                                          ctx.controls, ctx.fixedUpdateRate);
            ctx.session->beginConnect(ctx.connection.address(), ctx.connection.name);
        }

        if (ctx.menuControls.isPressed(UI::MenuAction::Back, ctx.input)) {
            ctx.session.reset();  // tells the server the player leaves
            return SceneId::Connect;
        }

        if (ctx.session->pollConnect())
            return SceneId::InGame;
    } catch (const Errors::Error& err) {
        return fail(ctx, err.message());  // what() is meant for a terminal
    } catch (const std::exception& err) {
        return fail(ctx, err.what());
    }
    return std::nullopt;
}

void RType::Scenes::Connecting::render(Context& ctx) const
{
    std::string text = "Connecting to " + ctx.connection.host + ":" + ctx.connection.port + "...";

    drawCenteredText(ctx.renderer, ctx.window, Wrecks::cstring_view(text), MESSAGE_Y, TEXT_SIZE);
    drawCenteredText(ctx.renderer, ctx.window, "Escape / B: Cancel", HINT_Y, TEXT_SIZE);
}
