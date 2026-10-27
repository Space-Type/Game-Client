/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Source file for
** Renderer class
*/

#include "Renderer.hpp"
#include "../Resources/Resources.hpp"
#include "../Resources/ResourcesImpl.hpp"
#include <cmath>

namespace {
    /**
     * Resolves a font handle into `out`: `custom` (the
     * looked-up font) or the built-in font for
     * FontId::Default. False for unknown handles.
     */
    bool resolveFont(RType::Render::FontId id, const Font* custom, Font& out) noexcept
    {
        if (id == RType::Render::FontId::Default) {
            out = GetFontDefault();
            return true;
        }
        if (custom == nullptr)
            return false;
        out = *custom;
        return true;
    }

    // Same spacing for drawing and measuring, so measures match
    float spacing(float size) noexcept
    {
        return size / 10.0F;
    }
}

RType::Render::Renderer::Renderer(Resources& resources) noexcept
    : m_Resources(resources)
{
}

void RType::Render::Renderer::drawSprite(TextureId texture, const SpriteParams& params) const
{
    const Texture2D* found = m_Resources.impl().texture(texture);
    if (found == nullptr)
        return;

    Rect source = params.source.value_or(Rect{0.0F, 0.0F, static_cast<float>(found->width),
                                              static_cast<float>(found->height)});
    Rect dest = {
        params.position.x,
        params.position.y,
        std::abs(source.width) * params.scale.x,
        std::abs(source.height) * params.scale.y,
    };
    ::Vector2 origin = {params.origin.x * dest.width, params.origin.y * dest.height};

    if (params.flipX)
        source.width = -source.width;
    if (params.flipY)
        source.height = -source.height;
    DrawTexturePro(*found, Convert::toRaylib(source), Convert::toRaylib(dest), origin,
                   params.rotation, Convert::toRaylib(params.tint));
}

void RType::Render::Renderer::drawScrolling(TextureId texture, Rect dest, Vec2D offset, float scale, Color tint) const
{
    const Texture2D* found = m_Resources.impl().texture(texture);
    if (found == nullptr || scale <= 0.0F)
        return;

    ::Rectangle source = {offset.x, offset.y, dest.width / scale, dest.height / scale};

    SetTextureWrap(*found, TEXTURE_WRAP_REPEAT);
    DrawTexturePro(*found, source, Convert::toRaylib(dest), {0.0F, 0.0F}, 0.0F, Convert::toRaylib(tint));
}

void RType::Render::Renderer::drawRect(Rect rect, Color color) const
{
    DrawRectangleRec(Convert::toRaylib(rect), Convert::toRaylib(color));
}

void RType::Render::Renderer::drawRectLines(Rect rect, float thickness, Color color) const
{
    DrawRectangleLinesEx(Convert::toRaylib(rect), thickness, Convert::toRaylib(color));
}

void RType::Render::Renderer::drawCircle(Vec2D center, float radius, Color color) const
{
    DrawCircleV(Convert::toRaylib(center), radius, Convert::toRaylib(color));
}

void RType::Render::Renderer::drawLine(Vec2D start, Vec2D end, float thickness, Color color) const
{
    DrawLineEx(Convert::toRaylib(start), Convert::toRaylib(end), thickness, Convert::toRaylib(color));
}

void RType::Render::Renderer::drawText(Wrecks::cstring_view text, Vec2D position, float size,
                                 Color color, FontId font) const
{
    Font resolved = {};
    if (!resolveFont(font, m_Resources.impl().font(font), resolved))
        return;

    DrawTextEx(resolved, text.data(), Convert::toRaylib(position), size, spacing(size), Convert::toRaylib(color));
}

RType::Render::Vec2D RType::Render::Renderer::measureText(Wrecks::cstring_view text, float size, FontId font) const
{
    Font resolved = {};
    if (!resolveFont(font, m_Resources.impl().font(font), resolved))
        return {};

    return Convert::fromRaylib(MeasureTextEx(resolved, text.data(), size, spacing(size)));
}
