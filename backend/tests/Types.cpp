/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Unit tests for
** backend value types
*/

#include <gtest/gtest.h>
#include <backend/Types.hpp>

// Everything is usable in constant expressions
static_assert(Backend::Vec2{1.0F, 2.0F} + Backend::Vec2{3.0F, 4.0F} == Backend::Vec2{4.0F, 6.0F});
static_assert(Backend::Vec2{1.0F, 2.0F} - Backend::Vec2{3.0F, 5.0F} == Backend::Vec2{-2.0F, -3.0F});
static_assert(Backend::Vec2{1.0F, -2.0F} * 3.0F == Backend::Vec2{3.0F, -6.0F});
static_assert(static_cast<std::uint32_t>(Backend::TextureId::Invalid) == 0);
static_assert(static_cast<std::uint32_t>(Backend::FontId::Default) == 0);
static_assert(static_cast<std::uint32_t>(Backend::SoundId::Invalid) == 0);
static_assert(static_cast<std::uint32_t>(Backend::MusicId::Invalid) == 0);

TEST(backend_Types, vec2Defaults)
{
    Backend::Vec2 vec;

    EXPECT_EQ(vec.x, 0.0F);
    EXPECT_EQ(vec.y, 0.0F);
}

TEST(backend_Types, vec2Operators)
{
    Backend::Vec2 a = {1.5F, -2.0F};
    Backend::Vec2 b = {0.5F, 4.0F};

    EXPECT_EQ(a + b, (Backend::Vec2{2.0F, 2.0F}));
    EXPECT_EQ(a - b, (Backend::Vec2{1.0F, -6.0F}));
    EXPECT_EQ(a * 2.0F, (Backend::Vec2{3.0F, -4.0F}));
    EXPECT_EQ(a * 0.0F, (Backend::Vec2{0.0F, -0.0F}));
}

TEST(backend_Types, vec2Equality)
{
    EXPECT_EQ((Backend::Vec2{1.0F, 2.0F}), (Backend::Vec2{1.0F, 2.0F}));
    EXPECT_NE((Backend::Vec2{1.0F, 2.0F}), (Backend::Vec2{2.0F, 1.0F}));
    EXPECT_NE((Backend::Vec2{1.0F, 2.0F}), (Backend::Vec2{1.0F, 2.5F}));
}

TEST(backend_Types, rectEquality)
{
    EXPECT_EQ(Backend::Rect{}, (Backend::Rect{0.0F, 0.0F, 0.0F, 0.0F}));
    EXPECT_EQ((Backend::Rect{1.0F, 2.0F, 3.0F, 4.0F}), (Backend::Rect{1.0F, 2.0F, 3.0F, 4.0F}));
    EXPECT_NE((Backend::Rect{1.0F, 2.0F, 3.0F, 4.0F}), (Backend::Rect{1.0F, 2.0F, 3.0F, 5.0F}));
}

TEST(backend_Types, colorDefaultsToOpaqueBlack)
{
    EXPECT_EQ(Backend::Color{}, Backend::Colors::BLACK);
    EXPECT_EQ(Backend::Color{}.a, 255);
    EXPECT_EQ(Backend::Colors::INVISIBLE.a, 0);
    EXPECT_NE(Backend::Colors::WHITE, Backend::Colors::BLACK);
}

TEST(backend_Types, spriteParamsDefaults)
{
    Backend::SpriteParams params;

    EXPECT_EQ(params.position, Backend::Vec2{});
    EXPECT_FALSE(params.source.has_value());
    EXPECT_EQ(params.origin, Backend::Vec2{});
    EXPECT_EQ(params.rotation, 0.0F);
    EXPECT_EQ(params.scale, (Backend::Vec2{1.0F, 1.0F}));
    EXPECT_EQ(params.tint, Backend::Colors::WHITE);
    EXPECT_FALSE(params.flipX);
    EXPECT_FALSE(params.flipY);
}
