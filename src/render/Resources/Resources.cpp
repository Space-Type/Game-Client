/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** Resources class
*/

#include "../Error.hpp"
#include "Resources.hpp"
#include "ResourcesImpl.hpp"
#include <algorithm>

RType::Render::Resources::Resources(const Window&)
    : m_Impl(std::make_unique<Impl>())
{
}

RType::Render::Resources::~Resources()
{
    for (auto& [id, texture] : m_Impl->textures)
        UnloadTexture(texture);
    for (auto& [id, font] : m_Impl->fonts)
        UnloadFont(font);
}

RType::Render::TextureId RType::Render::Resources::loadTexture(const std::string& path)
{
    if (auto cached = m_Impl->texturePaths.find(path); cached != m_Impl->texturePaths.end())
        return static_cast<TextureId>(cached->second);

    Texture2D texture = LoadTexture(path.c_str());
    if (!IsTextureValid(texture))
        throw Error("Resources: cannot load texture '" + path + "'");

    std::uint32_t id = m_Impl->nextId;
    try {
        m_Impl->textures.emplace(id, texture);
        m_Impl->texturePaths.emplace(path, id);
    } catch (...) {
        m_Impl->textures.erase(id);
        UnloadTexture(texture);
        throw;
    }
    ++m_Impl->nextId;
    return static_cast<TextureId>(id);
}

RType::Render::FontId RType::Render::Resources::loadFont(const std::string& path, int size)
{
    if (size <= 0)
        throw Error("Resources: invalid size for font '" + path + "'");

    std::pair<std::string, int> key = {path, size};
    if (auto cached = m_Impl->fontPaths.find(key); cached != m_Impl->fontPaths.end())
        return static_cast<FontId>(cached->second);

    Font font = LoadFontEx(path.c_str(), size, nullptr, 0);
    if (!IsFontValid(font))
        throw Error("Resources: cannot load font '" + path + "'");
    // raylib silently falls back to its default font (unsupported
    // format, no glyph): not ours to unload, UnloadFont skips it anyway
    if (font.texture.id == GetFontDefault().texture.id)
        throw Error("Resources: cannot load font '" + path + "' (fell back to the default font)");

    std::uint32_t id = m_Impl->nextId;
    try {
        m_Impl->fonts.emplace(id, font);
        m_Impl->fontPaths.emplace(std::move(key), id);
    } catch (...) {
        m_Impl->fonts.erase(id);
        UnloadFont(font);
        throw;
    }
    ++m_Impl->nextId;
    return static_cast<FontId>(id);
}

void RType::Render::Resources::unload(TextureId texture) noexcept
{
    auto found = m_Impl->textures.find(static_cast<std::uint32_t>(texture));
    if (found == m_Impl->textures.end())
        return;

    UnloadTexture(found->second);
    m_Impl->textures.erase(found);
    std::erase_if(m_Impl->texturePaths, [texture](const auto& entry) {
        return entry.second == static_cast<std::uint32_t>(texture);
    });
}

void RType::Render::Resources::unload(FontId font) noexcept
{
    auto found = m_Impl->fonts.find(static_cast<std::uint32_t>(font));
    if (found == m_Impl->fonts.end())
        return;

    UnloadFont(found->second);
    m_Impl->fonts.erase(found);
    std::erase_if(m_Impl->fontPaths, [font](const auto& entry) {
        return entry.second == static_cast<std::uint32_t>(font);
    });
}

bool RType::Render::Resources::isLoaded(TextureId texture) const noexcept
{
    return m_Impl->texture(texture) != nullptr;
}

bool RType::Render::Resources::isLoaded(FontId font) const noexcept
{
    return font == FontId::Default || m_Impl->font(font) != nullptr;
}

RType::Render::Vec2D RType::Render::Resources::textureSize(TextureId texture) const noexcept
{
    const Texture2D* found = m_Impl->texture(texture);
    if (found == nullptr)
        return {};

    return {static_cast<float>(found->width), static_cast<float>(found->height)};
}

const RType::Render::Resources::Impl& RType::Render::Resources::impl() const noexcept
{
    return *m_Impl;
}
