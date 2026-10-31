/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Connect scene
*/

#include "Connect.hpp"
#include "../Text.hpp"
#include <cstddef>
#include <utility>
#include <variant>
#include <vector>


namespace {
    using RType::UI::Theme::LABEL_WIDTH;
    using RType::UI::Theme::MARKER_GAP;

    // Widgets, in Form order
    constexpr std::size_t HOST_FIELD = 0;
    constexpr std::size_t PORT_FIELD = 1;
    constexpr std::size_t NAME_FIELD = 2;
    constexpr std::size_t CONNECT_BUTTON = 3;
    constexpr std::size_t QUIT_BUTTON = 4;

    // Layout on the 1280x720 virtual screen, from top to bottom
    constexpr float TITLE_Y = 30.0F;
    constexpr float WIDGET_HEIGHT = 64.0F;
    constexpr float FIELD_X = 410.0F;     // marker at 100, label at 130
    constexpr float FIELD_WIDTH = 770.0F;
    constexpr float HOST_Y = 130.0F;
    constexpr float PORT_Y = 210.0F;
    constexpr float NAME_Y = 290.0F;
    constexpr float BUTTON_X = 480.0F;
    constexpr float BUTTON_WIDTH = 320.0F;
    constexpr float CONNECT_Y = 380.0F;
    constexpr float QUIT_Y = 460.0F;
    constexpr float MESSAGE_Y = 550.0F;
    constexpr float MESSAGE_SIZE = 28.0F;
    constexpr float HINT_Y = 650.0F;

    static_assert(FIELD_X - LABEL_WIDTH - MARKER_GAP >= 0.0F && FIELD_X + FIELD_WIDTH <= 1280.0F);

    RType::UI::Form buildForm(const RType::Game::ConnectionInfo& info)
    {
        using RType::Game::ConnectionInfo;
        using RType::UI::Button;
        using RType::UI::TextField;

        const auto field = [](float y){ return RType::Render::Rect{ .x = FIELD_X, .y = y, .width = FIELD_WIDTH, .height = WIDGET_HEIGHT }; };
        const auto button = [](float y){ return RType::Render::Rect{ .x = BUTTON_X, .y = y, .width = BUTTON_WIDTH, .height = WIDGET_HEIGHT }; };

        return RType::UI::Form(std::vector<RType::UI::Widget>{
            TextField("Server IP", info.host, ConnectionInfo::HOST_MAX_LENGTH, field(HOST_Y)),
            TextField("Port", info.port, ConnectionInfo::PORT_MAX_LENGTH, field(PORT_Y), RType::UI::isDigit),
            TextField("Nickname", info.name, ConnectionInfo::NAME_MAX_LENGTH, field(NAME_Y)),
            Button("Connect", button(CONNECT_Y)),
            Button("Quit", button(QUIT_Y)),
        });
    }

    const std::string& valueOf(const RType::UI::Form& form, std::size_t field)
    {
        return std::get<RType::UI::TextField>(form.widgets()[field]).value();
    }
}


auto RType::Scenes::Connect::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (!m_Form)
        m_Form.emplace(buildForm(ctx.connection));

    UI::FrameInput input = UI::readFrameInput(ctx.input, ctx.menuControls);
    std::optional<std::size_t> activated = m_Form->update(input);

    if (activated == QUIT_BUTTON || input.isPressed(UI::MenuAction::Back))
        return SceneId::Quit;

    if (activated != CONNECT_BUTTON)
        return std::nullopt;

    ctx.connection.host = valueOf(*m_Form, HOST_FIELD);
    ctx.connection.port = valueOf(*m_Form, PORT_FIELD);
    ctx.connection.name = valueOf(*m_Form, NAME_FIELD);

    std::optional<std::string> error = ctx.connection.validate();

    if (error) {
        m_Error = std::move(*error);
        return std::nullopt;
    }
    return SceneId::Connecting;
}

void RType::Scenes::Connect::render(Context& ctx) const
{
    drawCenteredText(ctx.renderer, ctx.window, "R-Type", TITLE_Y, TITLE_SIZE);

    if (m_Form)
        m_Form->draw(ctx.renderer);

    const std::string& message = m_Error.empty() ? ctx.message : m_Error;

    if (!message.empty())
        drawCenteredText(ctx.renderer, ctx.window, Wrecks::cstring_view(message), MESSAGE_Y, MESSAGE_SIZE, UI::Theme::ERROR_COLOR);

    drawCenteredText(ctx.renderer, ctx.window, "Tab / Arrows: Move    Enter / A: Confirm    Escape / B: Quit",
                     HINT_Y, UI::Theme::HINT_SIZE, UI::Theme::FOREGROUND);
}
