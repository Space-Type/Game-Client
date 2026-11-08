/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** the bosses the client knows
*/

#include <gtest/gtest.h>
#include <rtype/game.hpp>

using Protecs::Networking::Packets::EntityCreate;

TEST(game_Bosses, aCoreGivesItsBody)
{
    const auto* dobkeratops = RType::Game::bossOf(EntityCreate::ENEMY_DOBKERATOPS_CORE);
    const auto* gomander = RType::Game::bossOf(EntityCreate::ENEMY_GOMANDER_CORE);

    ASSERT_NE(dobkeratops, nullptr);
    ASSERT_NE(gomander, nullptr);
    EXPECT_EQ(dobkeratops->body, EntityCreate::ENEMY_DOBKERATOPS);
    EXPECT_EQ(dobkeratops->name, "DOBKERATOPS");
    EXPECT_EQ(gomander->body, EntityCreate::ENEMY_GOMANDER);
    EXPECT_EQ(gomander->name, "GOMANDER");
}

TEST(game_Bosses, otherKindsAreNotBosses)
{
    EXPECT_EQ(RType::Game::bossOf(EntityCreate::ENEMY_PATA_PATA), nullptr);
    EXPECT_EQ(RType::Game::bossOf(EntityCreate::ENEMY_GOMANDER), nullptr);  // the body is not the core
    EXPECT_TRUE(RType::Game::isBossBody(EntityCreate::ENEMY_GOMANDER));
    EXPECT_TRUE(RType::Game::isBossBody(EntityCreate::ENEMY_DOBKERATOPS));
    EXPECT_FALSE(RType::Game::isBossBody(EntityCreate::ENEMY_DOBKERATOPS_TAIL));
}
