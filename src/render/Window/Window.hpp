/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Window class
*/

#pragma once

#include "../Types.hpp"
#include <memory>
#include <string>

namespace RType::Render {

    struct WindowConfig {
        std::string title = "R-Type";
        int width = 1280;           // Initial window size, screen pixels
        int height = 720;
        int virtualWidth = 1920;    // Fixed drawing resolution, letterboxed into the window
        int virtualHeight = 1080;
        bool resizable = true;
        bool vsync = true;
        bool fullscreen = false;
    };

    /**
     * Window and frame lifecycle.
     *
     * All drawing happens in virtual coordinates
     * ([0, virtualWidth] x [0, virtualHeight]) between
     * `beginFrame()` and `endFrame()`; the frame is then
     * scaled and letterboxed into the real window.
     *
     * Only one Window may exist at a time (the
     * underlying library has global state). It must
     * outlive every Resources, Renderer and Input.
     *
     * The render module never paces time: no frame-rate
     * cap is applied, timing is up to the game loop.
     * The escape key does not close the window.
     */
    class Window {
        struct Impl;
        std::unique_ptr<Impl> m_Impl;

        public:
            /**
             * Throws RType::Render::Error if a Window already
             * exists or if the window cannot be created.
             */
            explicit Window(const WindowConfig& config);
            ~Window();

            Window(const Window&) = delete;
            Window(Window&&) = delete;
            Window& operator=(const Window&) = delete;
            Window& operator=(Window&&) = delete;

            // True once the user closed the window or `close()` was called
            bool shouldClose() const noexcept;
            void close() noexcept;
            void toggleFullscreen() noexcept;

            Vec2 size() const noexcept;             // Current window size, screen pixels
            Vec2 virtualSize() const noexcept;
            Rect viewport() const noexcept;         // Letterboxed area, screen pixels
            Vec2 screenToVirtual(Vec2 screen) const noexcept;

            // Starts drawing into the virtual frame, cleared with `clear`
            void beginFrame(Color clear = Colors::BLACK);

            // Presents the virtual frame letterboxed with `bars`, polls input events
            void endFrame(Color bars = Colors::BLACK);
    };
}
