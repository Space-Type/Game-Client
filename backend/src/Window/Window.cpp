/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Source file for
** Window class
*/

#include <backend/Error.hpp>
#include <backend/Letterbox.hpp>
#include <backend/Window.hpp>
#include "Convert.hpp"
#include "Log.hpp"
#include <atomic>

namespace {
    // raylib has a single global window
    std::atomic<bool> s_WindowExists = false;
}

struct Backend::Window::Impl {
    RenderTexture2D target = {};
    Vec2 virtualSize = {};
    bool closeRequested = false;
};

Backend::Window::Window(const WindowConfig& config)
{
    if (config.virtualWidth <= 0 || config.virtualHeight <= 0)
        throw Error("Window: virtual size must be strictly positive");
    if (s_WindowExists.exchange(true))
        throw Error("Window: a window already exists");

    try {
        m_Impl = std::make_unique<Impl>();
        m_Impl->virtualSize = {
            static_cast<float>(config.virtualWidth),
            static_cast<float>(config.virtualHeight),
        };

        unsigned int flags = 0;
        if (config.resizable)
            flags |= FLAG_WINDOW_RESIZABLE;
        if (config.vsync)
            flags |= FLAG_VSYNC_HINT;
        if (config.fullscreen)
            flags |= FLAG_FULLSCREEN_MODE;

        // Also keeps raylib from calling exit() on a fatal GL error
        Log::installCallback();
        SetConfigFlags(flags);
        InitWindow(config.width, config.height, config.title.c_str());
        if (!IsWindowReady())
            throw Error("Window: cannot create the window");
        SetExitKey(KEY_NULL);

        m_Impl->target = LoadRenderTexture(config.virtualWidth, config.virtualHeight);
        if (!IsRenderTextureValid(m_Impl->target)) {
            CloseWindow();
            throw Error("Window: cannot create the virtual frame");
        }
        SetTextureFilter(m_Impl->target.texture, TEXTURE_FILTER_BILINEAR);
    } catch (...) {
        s_WindowExists = false;
        throw;
    }
}

Backend::Window::~Window()
{
    UnloadRenderTexture(m_Impl->target);
    CloseWindow();
    s_WindowExists = false;
}

bool Backend::Window::shouldClose() const noexcept
{
    return m_Impl->closeRequested || WindowShouldClose();
}

void Backend::Window::close() noexcept
{
    m_Impl->closeRequested = true;
}

void Backend::Window::toggleFullscreen() noexcept
{
    ToggleFullscreen();
}

Backend::Vec2 Backend::Window::size() const noexcept
{
    return {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
}

Backend::Vec2 Backend::Window::virtualSize() const noexcept
{
    return m_Impl->virtualSize;
}

Backend::Rect Backend::Window::viewport() const noexcept
{
    return computeViewport(size(), m_Impl->virtualSize);
}

Backend::Vec2 Backend::Window::screenToVirtual(Vec2 screen) const noexcept
{
    return Backend::screenToVirtual(screen, viewport(), m_Impl->virtualSize);
}

void Backend::Window::beginFrame(Color clear)
{
    BeginTextureMode(m_Impl->target);
    ClearBackground(Convert::toRaylib(clear));
}

void Backend::Window::endFrame(Color bars)
{
    const Texture2D& frame = m_Impl->target.texture;
    // Render textures are stored upside down: flip the source
    ::Rectangle source = {0.0F, 0.0F, static_cast<float>(frame.width), -static_cast<float>(frame.height)};

    EndTextureMode();
    BeginDrawing();
    ClearBackground(Convert::toRaylib(bars));
    DrawTexturePro(frame, source, Convert::toRaylib(viewport()), {0.0F, 0.0F}, 0.0F, ::Color{255, 255, 255, 255});
    EndDrawing();
}
