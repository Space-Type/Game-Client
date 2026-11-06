/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Settings scene
*/

#include "Settings.hpp"
#include "../Text.hpp"
#include <array>
#include <cmath>
#include <variant>
#include <vector>


namespace {
    using RType::Scenes::Context;
    using RType::UI::Form;
    using RType::UI::FormEvent;

    /**
     * A page of settings. Its Form ends with a Back button,
     * handled by the scene: `apply` gets every other event.
     */
    struct Section {
        const char* title;
        Form (*build)(const Context& ctx);
        void (*apply)(Context& ctx, const Form& form, FormEvent event);
    };

    // Layout on the 1280x720 virtual screen
    constexpr float TITLE_Y = 30.0F;
    constexpr float WIDGET_HEIGHT = 64.0F;
    constexpr float ROW_STEP = 80.0F;
    constexpr float FIRST_ROW_Y = 150.0F;
    constexpr float FIELD_X = 410.0F;     // marker at 100, label at 130, as on the connect screen
    constexpr float FIELD_WIDTH = 770.0F;
    constexpr float BUTTON_X = 440.0F;
    constexpr float BUTTON_WIDTH = 400.0F;
    constexpr float HINT_Y = 650.0F;

    constexpr RType::Render::Rect field(std::size_t row)
    {
        return { .x = FIELD_X, .y = FIRST_ROW_Y + ROW_STEP * static_cast<float>(row), .width = FIELD_WIDTH, .height = WIDGET_HEIGHT };
    }

    constexpr RType::Render::Rect button(std::size_t row)
    {
        return { .x = BUTTON_X, .y = FIRST_ROW_Y + ROW_STEP * static_cast<float>(row), .width = BUTTON_WIDTH, .height = WIDGET_HEIGHT };
    }

    // Audio & display: widgets, in Form order
    constexpr std::size_t MASTER_SLIDER = 0;
    constexpr std::size_t MUSIC_SLIDER = 1;
    constexpr std::size_t EFFECTS_SLIDER = 2;
    constexpr std::size_t FULLSCREEN_BOX = 3;

    constexpr int VOLUME_STEP = 5;
    constexpr float PERCENT = 100.0F;

    RType::UI::Slider volumeSlider(const Context& ctx, const char* label, RType::Render::Channel channel, std::size_t row)
    {
        int percent = static_cast<int>(std::lround(ctx.audio.volume(channel) * PERCENT));

        return RType::UI::Slider(label, percent, 0, static_cast<int>(PERCENT), VOLUME_STEP, field(row), "%");
    }

    Form buildAudioDisplay(const Context& ctx)
    {
        using RType::Render::Channel;

        return Form(std::vector<RType::UI::Widget>{
            volumeSlider(ctx, "Master", Channel::Master, MASTER_SLIDER),
            volumeSlider(ctx, "Music", Channel::Music, MUSIC_SLIDER),
            volumeSlider(ctx, "Effects", Channel::Effects, EFFECTS_SLIDER),
            RType::UI::Checkbox("Fullscreen", ctx.window.isFullscreen(), field(FULLSCREEN_BOX)),
            RType::UI::Button("Back", button(FULLSCREEN_BOX + 1)),
        });
    }

    void applyAudioDisplay(Context& ctx, const Form& form, FormEvent event)
    {
        using RType::Render::Channel;

        const RType::UI::Widget& widget = form.widgets()[event.index];

        if (const auto* slider = std::get_if<RType::UI::Slider>(&widget)) {
            constexpr std::array CHANNELS = { Channel::Master, Channel::Music, Channel::Effects };

            ctx.audio.setVolume(CHANNELS[event.index - MASTER_SLIDER], static_cast<float>(slider->value()) / PERCENT);
        }
        if (const auto* box = std::get_if<RType::UI::Checkbox>(&widget); box && box->checked() != ctx.window.isFullscreen())
            ctx.window.toggleFullscreen();
    }

    // #43 and #44 add their sections here: the list shows one button per section
    constexpr std::array SECTIONS = {
        Section{ .title = "Audio & display", .build = buildAudioDisplay, .apply = applyAudioDisplay },
    };

    Form buildList()
    {
        std::vector<RType::UI::Widget> widgets;

        for (std::size_t i = 0; i < SECTIONS.size(); ++i)
            widgets.emplace_back(RType::UI::Button(SECTIONS[i].title, button(i)));
        widgets.emplace_back(RType::UI::Button("Back", button(SECTIONS.size())));
        return Form(std::move(widgets));
    }
}


void RType::Scenes::Settings::open(const Context& ctx, std::optional<std::size_t> page)
{
    m_Page = page;
    m_Form.emplace(page ? SECTIONS[*page].build(ctx) : buildList());
}

auto RType::Scenes::Settings::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (!m_Form)
        open(ctx, std::nullopt);

    UI::FrameInput input = UI::readFrameInput(ctx.input, ctx.menuControls);
    std::optional<FormEvent> event = m_Form->update(input);
    std::size_t back = m_Form->widgets().size() - 1;

    if (input.isPressed(UI::MenuAction::Back) || event == FormEvent{ .kind = FormEvent::Kind::Activated, .index = back }) {
        if (event)
            ctx.audio.play(ctx.clickSound);
        if (!m_Page)
            return ctx.settingsReturn;
        open(ctx, std::nullopt);
        return std::nullopt;
    }

    if (event && m_Page)
        SECTIONS[*m_Page].apply(ctx, *m_Form, *event);
    else if (event && event->kind == FormEvent::Kind::Activated)
        open(ctx, event->index);

    // After apply: the click is heard at the new Effects volume
    if (event)
        ctx.audio.play(ctx.clickSound);
    return std::nullopt;
}

void RType::Scenes::Settings::render(Context& ctx) const
{
    if (ctx.session)
        drawOverlay(ctx.renderer, ctx.window);

    drawCenteredText(ctx.renderer, ctx.window, m_Page ? SECTIONS[*m_Page].title : "Settings", TITLE_Y, TITLE_SIZE);

    if (m_Form)
        m_Form->draw(ctx.renderer);

    drawCenteredText(ctx.renderer, ctx.window, "Arrows / D-pad: Move and change   Escape / B: Back", HINT_Y, UI::Theme::HINT_SIZE);
}
