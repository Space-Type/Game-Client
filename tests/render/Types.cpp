/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Render value types
*/

#include <gtest/gtest.h>
#include <rtype/render.hpp>

// Everything is usable in constant expressions
static_assert(RType::Render::Vec2{1.0F, 2.0F} + RType::Render::Vec2{3.0F, 4.0F} == RType::Render::Vec2{4.0F, 6.0F});
static_assert(RType::Render::Vec2{1.0F, 2.0F} - RType::Render::Vec2{3.0F, 5.0F} == RType::Render::Vec2{-2.0F, -3.0F});
static_assert(RType::Render::Vec2{1.0F, -2.0F} * 3.0F == RType::Render::Vec2{3.0F, -6.0F});
static_assert(static_cast<std::uint32_t>(RType::Render::TextureId::Invalid) == 0);
static_assert(static_cast<std::uint32_t>(RType::Render::FontId::Default) == 0);
static_assert(static_cast<std::uint32_t>(RType::Render::SoundId::Invalid) == 0);
static_assert(static_cast<std::uint32_t>(RType::Render::MusicId::Invalid) == 0);

TEST(render_Types, vec2Defaults)
{
    RType::Render::Vec2D vec;

    EXPECT_EQ(vec.x, 0.0F);
    EXPECT_EQ(vec.y, 0.0F);
}

TEST(render_Types, vec2Operators)
{
    RType::Render::Vec2D a = {1.5F, -2.0F};
    RType::Render::Vec2D b = {0.5F, 4.0F};

    EXPECT_EQ(a + b, (RType::Render::Vec2{2.0F, 2.0F}));
    EXPECT_EQ(a - b, (RType::Render::Vec2{1.0F, -6.0F}));
    EXPECT_EQ(a * 2.0F, (RType::Render::Vec2{3.0F, -4.0F}));
    EXPECT_EQ(a * 0.0F, (RType::Render::Vec2{0.0F, -0.0F}));
}

TEST(render_Types, vec2Equality)
{
    EXPECT_EQ((RType::Render::Vec2{1.0F, 2.0F}), (RType::Render::Vec2{1.0F, 2.0F}));
    EXPECT_NE((RType::Render::Vec2{1.0F, 2.0F}), (RType::Render::Vec2{2.0F, 1.0F}));
    EXPECT_NE((RType::Render::Vec2{1.0F, 2.0F}), (RType::Render::Vec2{1.0F, 2.5F}));
}

TEST(render_Types, rectEquality)
{
    EXPECT_EQ(RType::Render::Rect{}, (RType::Render::Rect{0.0F, 0.0F, 0.0F, 0.0F}));
    EXPECT_EQ((RType::Render::Rect{1.0F, 2.0F, 3.0F, 4.0F}), (RType::Render::Rect{1.0F, 2.0F, 3.0F, 4.0F}));
    EXPECT_NE((RType::Render::Rect{1.0F, 2.0F, 3.0F, 4.0F}), (RType::Render::Rect{1.0F, 2.0F, 3.0F, 5.0F}));
}

TEST(render_Types, colorDefaultsToOpaqueBlack)
{
    EXPECT_EQ(RType::Render::Color{}, RType::Render::Colors::BLACK);
    EXPECT_EQ(RType::Render::Color{}.a, 255);
    EXPECT_EQ(RType::Render::Colors::INVISIBLE.a, 0);
    EXPECT_NE(RType::Render::Colors::WHITE, RType::Render::Colors::BLACK);
}

TEST(render_Types, spriteParamsDefaults)
{
    RType::Render::SpriteParams params;

    EXPECT_EQ(params.position, RType::Render::Vec2{});
    EXPECT_FALSE(params.source.has_value());
    EXPECT_EQ(params.origin, RType::Render::Vec2{});
    EXPECT_EQ(params.rotation, 0.0F);
    EXPECT_EQ(params.scale, (RType::Render::Vec2{1.0F, 1.0F}));
    EXPECT_EQ(params.tint, RType::Render::Colors::WHITE);
    EXPECT_FALSE(params.flipX);
    EXPECT_FALSE(params.flipY);
}
