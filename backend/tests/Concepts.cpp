/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Compile-time tests for
** backend concepts and headers
*/

#include <gtest/gtest.h>
#include <backend.hpp>

// Public headers must never leak raylib into game code
#ifdef RAYLIB_H
    #error "a public backend header includes raylib.h"
#endif

static_assert(Backend::RendererBackend<Backend::Renderer>);
static_assert(Backend::InputBackend<Backend::Input>);
static_assert(Backend::AudioBackend<Backend::Audio>);

static_assert(!Backend::RendererBackend<Backend::Input>);
static_assert(!Backend::InputBackend<Backend::Audio>);
static_assert(!Backend::AudioBackend<Backend::Renderer>);

// Single owners of global library state: never copied nor moved
static_assert(!std::is_copy_constructible_v<Backend::Window>);
static_assert(!std::is_move_constructible_v<Backend::Window>);
static_assert(!std::is_copy_constructible_v<Backend::Resources>);
static_assert(!std::is_move_constructible_v<Backend::Resources>);
static_assert(!std::is_copy_constructible_v<Backend::Audio>);
static_assert(!std::is_move_constructible_v<Backend::Audio>);

static_assert(std::is_base_of_v<Wrecks::Error, Backend::Error>);

TEST(backend_Concepts, satisfiedAtCompileTime)
{
    SUCCEED();
}

TEST(backend_Concepts, errorCarriesMessage)
{
    try {
        throw Backend::Error("boom");
    } catch (const Wrecks::Error& error) {
        EXPECT_STREQ(error.what(), "boom");
        return;
    }
    FAIL() << "Backend::Error not caught as Wrecks::Error";
}
