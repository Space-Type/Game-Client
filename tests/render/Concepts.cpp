/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Compile-time tests for
** Render concepts and headers
*/

#include <gtest/gtest.h>
#include <rtype/render.hpp>

// Public headers must never leak raylib into game code
#ifdef RAYLIB_H
    #error "a public render header includes raylib.h"
#endif

static_assert(RType::Render::RendererType<RType::Render::Renderer>);
static_assert(RType::Render::InputType<RType::Render::Input>);
static_assert(RType::Render::AudioType<RType::Render::Audio>);

static_assert(!RType::Render::RendererType<RType::Render::Input>);
static_assert(!RType::Render::InputType<RType::Render::Audio>);
static_assert(!RType::Render::AudioType<RType::Render::Renderer>);

// Single owners of global library state: never copied nor moved
static_assert(!std::is_copy_constructible_v<RType::Render::Window>);
static_assert(!std::is_move_constructible_v<RType::Render::Window>);
static_assert(!std::is_copy_constructible_v<RType::Render::Resources>);
static_assert(!std::is_move_constructible_v<RType::Render::Resources>);
static_assert(!std::is_copy_constructible_v<RType::Render::Audio>);
static_assert(!std::is_move_constructible_v<RType::Render::Audio>);

static_assert(std::is_base_of_v<Wrecks::Error, RType::Render::Error>);

TEST(render_Concepts, satisfiedAtCompileTime)
{
    SUCCEED();
}

TEST(render_Concepts, errorCarriesMessage)
{
    try {
        throw RType::Render::Error("boom");
    } catch (const Wrecks::Error& error) {
        EXPECT_STREQ(error.what(), "boom");
        return;
    }
    FAIL() << "RType::Render::Error not caught as Wrecks::Error";
}
