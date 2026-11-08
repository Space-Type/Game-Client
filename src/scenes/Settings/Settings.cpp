/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Implementation for
** Settings scene
*/

#include "Settings.hpp"
#include "../Text.hpp"
#include "../../game/Preferences/Preferences.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <string>
#include <variant>
#include <vector>


namespace {
    using RType::Scenes::Context;
    using RType::UI::Form;
    using RType::UI::FormEvent;
    using Capture = RType::Scenes::Settings::Capture;

    /**
     * A page of settings. Its Form ends with a Back button,
     * handled by the scene: `apply` gets every other event
     * and returns true when the page must be built again
     * (its labels changed).
     * A button for which `capture` returns a control slot
     * waits for the next input instead, then the scene
     * rebinds the slot. `draw` adds text around the Form.
     */
    struct Section {
        const char* title;
        Form (*build)(const Context& ctx);
        bool (*apply)(Context& ctx, const Form& form, FormEvent event);
        std::optional<Capture> (*capture)(std::size_t index) = nullptr;
        void (*draw)(const Context& ctx) = nullptr;
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

    bool applyAudioDisplay(Context& ctx, const Form& form, FormEvent event)
    {
        using RType::Render::Channel;

        const RType::UI::Widget& widget = form.widgets()[event.index];

        if (const auto* slider = std::get_if<RType::UI::Slider>(&widget)) {
            constexpr std::array CHANNELS = { Channel::Master, Channel::Music, Channel::Effects };

            ctx.audio.setVolume(CHANNELS[event.index - MASTER_SLIDER], static_cast<float>(slider->value()) / PERCENT);
        }
        if (const auto* box = std::get_if<RType::UI::Checkbox>(&widget); box && box->checked() != ctx.window.isFullscreen())
            ctx.window.toggleFullscreen();
        return false;
    }

    // Controls: a keyboard / mouse and a gamepad button per action, then Reset and Back
    constexpr std::size_t SLOT_COUNT = 2;
    constexpr std::size_t RESET_BUTTON = RType::Game::ACTIONS.size() * SLOT_COUNT;

    constexpr float CONTROL_HEIGHT = 48.0F;  // still a big target (accessibility), 6 actions fit above the hint
    constexpr float CONTROL_STEP = 56.0F;
    constexpr float HEADER_Y = 110.0F;
    constexpr float FIRST_CONTROL_Y = 145.0F;
    constexpr float ACTION_X = 130.0F;
    constexpr float KEYBOARD_X = 400.0F;
    constexpr float GAMEPAD_X = 780.0F;
    constexpr float SLOT_WIDTH = 340.0F;
    constexpr float RESET_Y = FIRST_CONTROL_Y + CONTROL_STEP * (RType::Game::ACTIONS.size() + 0.3F);
    constexpr float CONFLICT_Y = RESET_Y + CONTROL_STEP * 2.0F + 8.0F;

    static_assert(GAMEPAD_X + SLOT_WIDTH <= 1280.0F && CONFLICT_Y + RType::UI::Theme::HINT_SIZE < HINT_Y);

    constexpr float controlY(std::size_t row)
    {
        return FIRST_CONTROL_Y + CONTROL_STEP * static_cast<float>(row);
    }

    std::optional<Capture> controlAt(std::size_t index)
    {
        if (index >= RESET_BUTTON)
            return std::nullopt;
        return Capture{
            .action = RType::Game::ACTIONS[index / SLOT_COUNT],
            .slot = index % SLOT_COUNT == 0 ? RType::Game::Slot::Keyboard : RType::Game::Slot::Gamepad,
        };
    }

    // True if another action also uses the binding
    bool isShared(const RType::Game::Controls& controls, RType::Game::Action action, const RType::Render::Binding& binding)
    {
        return std::ranges::any_of(RType::Game::ACTIONS, [&](RType::Game::Action other) {
            return other != action && std::ranges::find(controls.bindings(other), binding) != controls.bindings(other).end();
        });
    }

    std::string slotLabel(const RType::Game::Controls& controls, Capture control)
    {
        std::optional<RType::Render::Binding> binding = RType::Game::slotBinding(controls, control.action, control.slot);

        if (!binding)
            return "None";

        std::string label(RType::Game::bindingName(*binding));

        if (isShared(controls, control.action, *binding))
            label += " (!)";  // not only the colour: see the line under the table
        return label;
    }

    Form buildControls(const Context& ctx)
    {
        std::vector<RType::UI::Widget> widgets;

        for (std::size_t i = 0; i < RESET_BUTTON; ++i) {
            Capture control = *controlAt(i);
            float x = control.slot == RType::Game::Slot::Keyboard ? KEYBOARD_X : GAMEPAD_X;

            widgets.emplace_back(RType::UI::Button(slotLabel(ctx.controls, control),
                                                   { .x = x, .y = controlY(i / SLOT_COUNT), .width = SLOT_WIDTH, .height = CONTROL_HEIGHT }));
        }
        widgets.emplace_back(RType::UI::Button("Reset to defaults", { .x = BUTTON_X, .y = RESET_Y, .width = BUTTON_WIDTH, .height = CONTROL_HEIGHT }));
        widgets.emplace_back(RType::UI::Button("Back", { .x = BUTTON_X, .y = RESET_Y + CONTROL_STEP, .width = BUTTON_WIDTH, .height = CONTROL_HEIGHT }));
        return Form(std::move(widgets));
    }

    bool applyControls(Context& ctx, const Form&, FormEvent event)
    {
        if (event != FormEvent{ .kind = FormEvent::Kind::Activated, .index = RESET_BUTTON })
            return false;

        int gamepad = ctx.controls.gamepad();

        ctx.controls = RType::Game::defaultControls();
        ctx.controls.setGamepad(gamepad);
        return true;
    }

    void drawControls(const Context& ctx)
    {
        using RType::UI::Theme::HINT_SIZE;

        const float textOffset = (CONTROL_HEIGHT - RType::UI::Theme::TEXT_SIZE) / 2.0F;
        std::string conflicts;

        ctx.renderer.drawText("Keyboard / mouse", { .x = KEYBOARD_X, .y = HEADER_Y }, HINT_SIZE, RType::UI::Theme::FOREGROUND);
        ctx.renderer.drawText("Gamepad", { .x = GAMEPAD_X, .y = HEADER_Y }, HINT_SIZE, RType::UI::Theme::FOREGROUND);

        for (std::size_t row = 0; row < RType::Game::ACTIONS.size(); ++row) {
            RType::Game::Action action = RType::Game::ACTIONS[row];

            ctx.renderer.drawText(RType::Game::actionName(action), { .x = ACTION_X, .y = controlY(row) + textOffset },
                                  RType::UI::Theme::TEXT_SIZE, RType::UI::Theme::FOREGROUND);
            if (RType::Game::conflicts(ctx.controls, action))
                conflicts += (conflicts.empty() ? "" : ", ") + std::string(RType::Game::actionName(action));
        }

        if (!conflicts.empty())
            RType::Scenes::drawCenteredText(ctx.renderer, ctx.window, "(!) Same input on: " + conflicts, CONFLICT_Y, HINT_SIZE, RType::UI::Theme::ERROR_COLOR);
    }

    // Accessibility (#44): options that change how the game is displayed
    constexpr std::size_t HIGH_CONTRAST_BOX = 0;

    Form buildAccessibility(const Context& ctx)
    {
        return Form(std::vector<RType::UI::Widget>{
            RType::UI::Checkbox("High contrast", ctx.displayOptions.highContrast, field(HIGH_CONTRAST_BOX)),
            RType::UI::Button("Back", button(HIGH_CONTRAST_BOX + 1)),
        });
    }

    bool applyAccessibility(Context& ctx, const Form& form, FormEvent event)
    {
        if (const auto* box = std::get_if<RType::UI::Checkbox>(&form.widgets()[event.index]); box && event.index == HIGH_CONTRAST_BOX)
            ctx.displayOptions.highContrast = box->checked();
        return false;
    }

    // The list shows one button per section
    constexpr std::array SECTIONS = {
        Section{ .title = "Audio & display", .build = buildAudioDisplay, .apply = applyAudioDisplay },
        Section{ .title = "Controls", .build = buildControls, .apply = applyControls, .capture = controlAt, .draw = drawControls },
        Section{ .title = "Accessibility", .build = buildAccessibility, .apply = applyAccessibility },
    };

    // The input for a control slot pressed this frame, if any (Escape and Start excluded: they cancel)
    std::optional<RType::Render::Binding> pressedBinding(RType::Render::Input& input, int gamepad, RType::Game::Slot slot)
    {
        using RType::Render::GamepadButton;
        using RType::Render::MouseButton;

        if (slot == RType::Game::Slot::Keyboard) {
            while (std::optional<RType::Render::Key> key = input.nextPressed())
                if (*key != RType::Render::Key::Escape)
                    return *key;
            for (std::size_t i = 0; i < static_cast<std::size_t>(MouseButton::Count); ++i)
                if (input.isPressed(static_cast<MouseButton>(i)))
                    return static_cast<MouseButton>(i);
            return std::nullopt;
        }
        for (std::size_t i = 0; i < static_cast<std::size_t>(GamepadButton::Count); ++i) {
            auto button = static_cast<GamepadButton>(i);

            if (button != GamepadButton::Start && input.isPressed(gamepad, button))
                return button;
        }
        return std::nullopt;
    }

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

void RType::Scenes::Settings::updateCapture(Context& ctx)
{
    int gamepad = ctx.controls.gamepad();

    if (ctx.input.isPressed(Render::Key::Escape) || ctx.input.isPressed(gamepad, Render::GamepadButton::Start)) {
        m_Capture.reset();
        return;
    }

    std::optional<Render::Binding> binding = pressedBinding(ctx.input, gamepad, m_Capture->slot);

    if (!binding)
        return;

    std::size_t focus = m_Form->focus();

    Game::rebind(ctx.controls, m_Capture->action, m_Capture->slot, *binding);
    m_Capture.reset();
    open(ctx, m_Page);
    m_Form->setFocus(focus);
    ctx.audio.play(ctx.clickSound);
}

auto RType::Scenes::Settings::update(Context& ctx, Wrecks::DeltaTime) -> std::optional<SceneId>
{
    if (!m_Form)
        open(ctx, std::nullopt);

    // The Form gets no input meanwhile: the next key, button or click is the new binding
    if (m_Capture) {
        updateCapture(ctx);
        return std::nullopt;
    }

    UI::FrameInput input = UI::readFrameInput(ctx.input, ctx.menuControls);
    std::optional<FormEvent> event = m_Form->update(input);
    std::size_t back = m_Form->widgets().size() - 1;

    if (input.isPressed(UI::MenuAction::Back) || event == FormEvent{ .kind = FormEvent::Kind::Activated, .index = back }) {
        if (event)
            ctx.audio.play(ctx.clickSound);
        if (!m_Page) {
            // A failure is reported on stderr: the changes still hold until the game is closed
            Game::savePreferences(Game::currentPreferences(ctx.audio, ctx.window, ctx.controls, ctx.displayOptions));
            return ctx.settingsReturn;
        }
        open(ctx, std::nullopt);
        return std::nullopt;
    }

    if (event && m_Page) {
        const Section& section = SECTIONS[*m_Page];
        bool activated = event->kind == FormEvent::Kind::Activated;

        if (activated && section.capture)
            m_Capture = section.capture(event->index);
        if (!m_Capture && section.apply(ctx, *m_Form, *event)) {
            std::size_t focus = m_Form->focus();

            open(ctx, m_Page);
            m_Form->setFocus(focus);
        }
    } else if (event && event->kind == FormEvent::Kind::Activated)
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
    if (m_Page && SECTIONS[*m_Page].draw)
        SECTIONS[*m_Page].draw(ctx);

    if (m_Capture) {
        std::string prompt = std::string(m_Capture->slot == Game::Slot::Keyboard ? "Press a key or mouse button" : "Press a gamepad button")
                           + " for " + std::string(Game::actionName(m_Capture->action)) + "   Escape / Start: Cancel";

        drawCenteredText(ctx.renderer, ctx.window, prompt, HINT_Y, UI::Theme::HINT_SIZE, UI::Theme::FOCUS_COLOR);
        return;
    }
    drawCenteredText(ctx.renderer, ctx.window, "Arrows / D-pad: Move and change   Escape / B: Back", HINT_Y, UI::Theme::HINT_SIZE);
}
