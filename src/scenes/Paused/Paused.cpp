/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Paused scene
*/

#include "Paused.hpp"
#include "../Text.hpp"
#include <cstddef>
#include <vector>


namespace {
    // Buttons, in Form order
    constexpr std::size_t RESUME_BUTTON = 0;
    constexpr std::size_t SETTINGS_BUTTON = 1;
    constexpr std::size_t DISCONNECT_BUTTON = 2;

    // Layout on the 1280x720 virtual screen
    constexpr float TITLE_Y = 150.0F;
    constexpr float BUTTON_X = 480.0F;
    constexpr float BUTTON_WIDTH = 320.0F;
    constexpr float BUTTON_HEIGHT = 64.0F;
    constexpr float FIRST_BUTTON_Y = 300.0F;
    constexpr float BUTTON_STEP = 80.0F;
    constexpr float HINT_Y = 650.0F;

    RType::UI::Form buildForm()
    {
        const auto button = [](const char* label, std::size_t row){
            return RType::UI::Button(label, {
                .x = BUTTON_X,
                .y = FIRST_BUTTON_Y + BUTTON_STEP * static_cast<float>(row),
                .width = BUTTON_WIDTH,
                .height = BUTTON_HEIGHT,
            });
        };

        return RType::UI::Form(std::vector<RType::UI::Widget>{
            button("Resume", RESUME_BUTTON),
            button("Settings", SETTINGS_BUTTON),
            button("Disconnect", DISCONNECT_BUTTON),
        });
    }
}


auto RType::Scenes::Paused::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (!m_Form)
        m_Form.emplace(buildForm());

    UI::FrameInput input = UI::readFrameInput(ctx.input, ctx.menuControls);
    std::optional<UI::FormEvent> event = m_Form->update(input);

    if (event)
        ctx.audio.play(ctx.clickSound);

    auto activated = [&](std::size_t button) {
        return event == UI::FormEvent{ .kind = UI::FormEvent::Kind::Activated, .index = button };
    };
    bool start = ctx.input.isPressed(ctx.controls.gamepad(), Render::GamepadButton::Start);

    if (activated(RESUME_BUTTON) || input.isPressed(UI::MenuAction::Back) || start)
        return SceneId::InGame;
    if (activated(SETTINGS_BUTTON)) {
        ctx.settingsReturn = SceneId::Paused;
        return SceneId::Settings;
    }
    if (activated(DISCONNECT_BUTTON)) {
        ctx.session.reset();  // the client disconnects on destruction
        return SceneId::Connect;
    }
    return std::nullopt;
}

void RType::Scenes::Paused::render(Context& ctx) const
{
    drawOverlay(ctx.renderer, ctx.window);
    drawCenteredText(ctx.renderer, ctx.window, "Paused", TITLE_Y, TITLE_SIZE);

    if (m_Form)
        m_Form->draw(ctx.renderer);

    drawCenteredText(ctx.renderer, ctx.window, "Escape / B / Start: Resume", HINT_Y, UI::Theme::HINT_SIZE);
}
