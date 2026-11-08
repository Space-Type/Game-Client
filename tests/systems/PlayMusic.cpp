/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** PlayMusic system
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>
#include <chrono>

using namespace std::chrono_literals;

// Without an audio device (setAudio never called), a soundtrack changes nothing and does not crash
TEST(systems_PlayMusic, noAudioDoesNothing)
{
    RType::Game::Engine engine;
    auto soundtrack = engine.createEntity();

    engine.addComponent(soundtrack, RType::Components::Music{ .track = "musics/title.ogg" });
    engine.update(10ms);
    engine.get<RType::Components::Music>()[soundtrack].track = "musics/stage-1.ogg";
    engine.update(10ms);
    EXPECT_EQ(engine.get<RType::Components::Music>()[soundtrack].track, "musics/stage-1.ogg");
}
