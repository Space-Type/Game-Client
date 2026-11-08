/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** Freshness component
*/

#include <gtest/gtest.h>
#include <rtype/components.hpp>

using RType::Components::isStale;
using RType::Components::STALE_AFTER;

TEST(components_Freshness, updatedEntityIsKept)
{
    EXPECT_FALSE(isStale(0.0F, 0.0F));
    EXPECT_FALSE(isStale(STALE_AFTER, 0.0F));
}

TEST(components_Freshness, entityTheServerStoppedUpdatingIsStale)
{
    EXPECT_TRUE(isStale(STALE_AFTER + 0.1F, 0.0F));
}

TEST(components_Freshness, nothingIsStaleWhenNothingArrives)
{
    // Server or network hiccup: every entity misses its updates, none is deleted
    EXPECT_FALSE(isStale(STALE_AFTER + 5.0F, STALE_AFTER + 5.0F));
}
