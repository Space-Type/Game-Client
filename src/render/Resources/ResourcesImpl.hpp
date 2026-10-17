/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Private header for
** Resources implementation
*/

#pragma once

#include "Resources.hpp"
#include "../Convert.hpp"
#include <cstdint>
#include <map>
#include <string>
#include <unordered_map>
#include <utility>

/**
 * Handle tables shared by Resources and Renderer.
 * Texture and font ids come from the same counter,
 * so a given id is never reused for another resource.
 */
struct RType::Render::Resources::Impl {
    std::uint32_t nextId = 1;

    std::unordered_map<std::uint32_t, Texture2D> textures;
    std::unordered_map<std::string, std::uint32_t> texturePaths;

    std::unordered_map<std::uint32_t, Font> fonts;
    std::map<std::pair<std::string, int>, std::uint32_t> fontPaths;

    // nullptr for unknown handles
    const Texture2D* texture(TextureId id) const noexcept
    {
        auto found = textures.find(static_cast<std::uint32_t>(id));

        return found == textures.end() ? nullptr : &found->second;
    }

    // nullptr for unknown handles, FontId::Default included
    const Font* font(FontId id) const noexcept
    {
        auto found = fonts.find(static_cast<std::uint32_t>(id));

        return found == fonts.end() ? nullptr : &found->second;
    }
};
