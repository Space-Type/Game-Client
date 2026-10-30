/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** scene Manager class
*/

#include <gtest/gtest.h>
#include <rtype/scenes.hpp>
#include <optional>
#include <string>
#include <vector>

namespace {

    using RType::Scenes::SceneId;

    /** Records the calls of the scenes, scripts the value returned by their update */
    struct FakeContext {
        std::vector<std::string> calls;
        std::optional<SceneId> next;
    };

    /** Counts its own updates: shows when a scene is constructed anew */
    template <SceneId ID>
    struct FakeScene {
        int updates = 0;

        std::optional<SceneId> update(FakeContext& ctx, Wrecks::DeltaTime)
        {
            updates++;
            ctx.calls.push_back("update " + std::to_string(static_cast<int>(ID)) + " " + std::to_string(updates));
            return ctx.next;
        }

        void render(FakeContext& ctx) const
        {
            ctx.calls.push_back("render " + std::to_string(static_cast<int>(ID)) + " " + std::to_string(updates));
        }
    };

    struct NotAScene {
        void update(FakeContext&, Wrecks::DeltaTime) {}
    };

    static_assert(RType::Scenes::Scene<FakeScene<SceneId::Menu>, FakeContext>);
    static_assert(!RType::Scenes::Scene<NotAScene, FakeContext>);

    using Manager = RType::Scenes::Manager<FakeContext,
                                           FakeScene<SceneId::Menu>,
                                           FakeScene<SceneId::Connecting>,
                                           FakeScene<SceneId::InGame>,
                                           FakeScene<SceneId::Paused>,
                                           FakeScene<SceneId::GameOver>>;

    constexpr Wrecks::DeltaTime DT = Wrecks::DeltaTime(16.0);
}


TEST(scenes_Manager, initialSceneIsMenu)
{
    Manager scenes;

    EXPECT_EQ(scenes.current(), SceneId::Menu);
    EXPECT_FALSE(scenes.quitRequested());
}

TEST(scenes_Manager, applyPendingWithoutRequestIsNoop)
{
    Manager scenes;

    EXPECT_FALSE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::Menu);
}

TEST(scenes_Manager, requestIsDeferred)
{
    Manager scenes;
    FakeContext ctx;

    scenes.request(SceneId::InGame);
    EXPECT_EQ(scenes.current(), SceneId::Menu);

    scenes.update(ctx, DT);
    scenes.render(ctx);
    const std::vector<std::string> expected = {"update 0 1", "render 0 1"};
    EXPECT_EQ(ctx.calls, expected);

    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::InGame);
    EXPECT_FALSE(scenes.applyPending());
}

TEST(scenes_Manager, lastRequestWins)
{
    Manager scenes;

    scenes.request(SceneId::InGame);
    scenes.request(SceneId::GameOver);
    scenes.request(SceneId::Paused);

    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::Paused);
}

TEST(scenes_Manager, applyPendingRecreatesScene)
{
    Manager scenes;
    FakeContext ctx;

    scenes.update(ctx, DT);
    scenes.update(ctx, DT);
    scenes.request(SceneId::Menu);
    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::Menu);

    ctx.calls.clear();
    scenes.render(ctx);
    scenes.update(ctx, DT);
    const std::vector<std::string> expected = {"render 0 0", "update 0 1"};
    EXPECT_EQ(ctx.calls, expected);
}

TEST(scenes_Manager, quitKeepsCurrentScene)
{
    Manager scenes;
    FakeContext ctx;

    scenes.request(SceneId::GameOver);
    scenes.applyPending();
    scenes.update(ctx, DT);

    scenes.request(SceneId::Quit);
    EXPECT_FALSE(scenes.quitRequested());
    EXPECT_TRUE(scenes.applyPending());
    EXPECT_TRUE(scenes.quitRequested());
    EXPECT_EQ(scenes.current(), SceneId::GameOver);

    ctx.calls.clear();
    scenes.render(ctx);
    const std::vector<std::string> expected = {"render 4 1"};
    EXPECT_EQ(ctx.calls, expected);
}

TEST(scenes_Manager, forwardsToActiveScene)
{
    Manager scenes;
    FakeContext ctx;

    scenes.request(SceneId::Connecting);
    scenes.applyPending();
    scenes.update(ctx, DT);
    scenes.render(ctx);

    scenes.request(SceneId::InGame);
    scenes.applyPending();
    scenes.update(ctx, DT);
    scenes.update(ctx, DT);
    scenes.render(ctx);

    const std::vector<std::string> expected = {
        "update 1 1", "render 1 1", "update 2 1", "update 2 2", "render 2 2",
    };
    EXPECT_EQ(ctx.calls, expected);
}

TEST(scenes_Manager, updateResultBecomesRequest)
{
    Manager scenes;
    FakeContext ctx;

    ctx.next = SceneId::Connecting;
    EXPECT_EQ(scenes.update(ctx, DT), SceneId::Connecting);
    EXPECT_EQ(scenes.current(), SceneId::Menu);
    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::Connecting);

    ctx.next = std::nullopt;
    EXPECT_EQ(scenes.update(ctx, DT), std::nullopt);
    EXPECT_FALSE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::Connecting);
}

TEST(scenes_Manager, updateResultOverridesEarlierRequest)
{
    Manager scenes;
    FakeContext ctx;

    scenes.request(SceneId::GameOver);
    ctx.next = SceneId::InGame;
    scenes.update(ctx, DT);

    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::InGame);
}

TEST(scenes_Manager, requestAfterUpdateOverridesResult)
{
    Manager scenes;
    FakeContext ctx;

    ctx.next = SceneId::Paused;
    scenes.update(ctx, DT);
    scenes.request(SceneId::GameOver);

    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::GameOver);
}

TEST(scenes_Manager, unknownSceneIdIsIgnored)
{
    Manager scenes;

    scenes.request(SceneId::InGame);
    scenes.applyPending();
    scenes.request(static_cast<SceneId>(42));

    EXPECT_FALSE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::InGame);
    EXPECT_FALSE(scenes.quitRequested());
    EXPECT_FALSE(scenes.applyPending());
}

TEST(scenes_Manager, quitStaysRequested)
{
    Manager scenes;

    scenes.request(SceneId::Quit);
    scenes.applyPending();
    scenes.request(SceneId::Menu);
    scenes.applyPending();

    EXPECT_TRUE(scenes.quitRequested());
}

TEST(scenes_Manager, updateWithoutResultKeepsEarlierRequest)
{
    Manager scenes;
    FakeContext ctx;

    scenes.request(SceneId::GameOver);
    EXPECT_EQ(scenes.update(ctx, DT), std::nullopt);

    EXPECT_TRUE(scenes.applyPending());
    EXPECT_EQ(scenes.current(), SceneId::GameOver);
}
