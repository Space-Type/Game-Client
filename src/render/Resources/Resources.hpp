/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Header file for
** Resources class
*/

#pragma once

#include "../Types.hpp"
#include <memory>
#include <string>

namespace RType::Render {

    class Window;

    /**
     * Owns graphical resources (textures, fonts) and
     * hands out plain handles to them.
     *
     * Loads are cached: loading the same path twice
     * returns the same handle. Everything still loaded
     * is released by the destructor.
     *
     * Requires a live Window, which must outlive it.
     */
    class Resources {
        public:
            /**
             * Handle tables. Opaque: only defined in the
             * private ResourcesImpl.hpp, so only the render
             * module itself (the Renderer) can use them.
             */
            struct Impl;

        private:
            std::unique_ptr<Impl> m_Impl;

        public:
            explicit Resources(const Window& window);
            ~Resources();

            Resources(const Resources&) = delete;
            Resources(Resources&&) = delete;
            Resources& operator=(const Resources&) = delete;
            Resources& operator=(Resources&&) = delete;

            // Throws RType::Render::Error if the file cannot be loaded
            TextureId loadTexture(const std::string& path);

            // Cached by (path, size). Throws RType::Render::Error if the file cannot be loaded
            FontId loadFont(const std::string& path, int size);

            // No-op for unknown, already unloaded, Invalid or Default handles
            void unload(TextureId texture) noexcept;
            void unload(FontId font) noexcept;

            bool isLoaded(TextureId texture) const noexcept;
            bool isLoaded(FontId font) const noexcept;  // True for FontId::Default

            // {0, 0} for unknown handles
            Vec2 textureSize(TextureId texture) const noexcept;

            // Read-only access to the handle tables (see Impl)
            const Impl& impl() const noexcept;
    };
}
