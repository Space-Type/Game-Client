/*
** EPITECH PROJECT, 2026
** Project - RType
** File description:
** Unit tests for
** letterbox math
*/

#include <gtest/gtest.h>
#include <rtype/render.hpp>
#include <limits>

namespace {
    constexpr RType::Render::Vec2 VIRTUAL = {1920.0F, 1080.0F};

    void expectNear(RType::Render::Vec2 actual, RType::Render::Vec2 expected)
    {
        EXPECT_NEAR(actual.x, expected.x, 1e-3F);
        EXPECT_NEAR(actual.y, expected.y, 1e-3F);
    }
}


/////////////////////////////////////////////////////////
//                                                     //
//                   computeViewport                   //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_Letterbox, sameSizeFillsWindow)
{
    EXPECT_EQ(RType::Render::computeViewport(VIRTUAL, VIRTUAL), (RType::Render::Rect{0.0F, 0.0F, 1920.0F, 1080.0F}));
}

TEST(render_Letterbox, sameAspectScalesDown)
{
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, VIRTUAL), (RType::Render::Rect{0.0F, 0.0F, 1280.0F, 720.0F}));
}

TEST(render_Letterbox, sameAspectScalesUp)
{
    EXPECT_EQ(RType::Render::computeViewport({3840.0F, 2160.0F}, VIRTUAL), (RType::Render::Rect{0.0F, 0.0F, 3840.0F, 2160.0F}));
}

TEST(render_Letterbox, widerWindowGetsSideBars)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    EXPECT_EQ(viewport, (RType::Render::Rect{320.0F, 0.0F, 1920.0F, 1080.0F}));
}

TEST(render_Letterbox, tallerWindowGetsTopBottomBars)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({1920.0F, 1440.0F}, VIRTUAL);

    EXPECT_EQ(viewport, (RType::Render::Rect{0.0F, 180.0F, 1920.0F, 1080.0F}));
}

TEST(render_Letterbox, squareWindowKeepsAspect)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({800.0F, 800.0F}, VIRTUAL);

    EXPECT_FLOAT_EQ(viewport.x, 0.0F);
    EXPECT_FLOAT_EQ(viewport.width, 800.0F);
    EXPECT_FLOAT_EQ(viewport.height, 450.0F);
    EXPECT_FLOAT_EQ(viewport.y, 175.0F);
}

TEST(render_Letterbox, viewportIsCentered)
{
    RType::Render::Vec2 window = {1000.0F, 300.0F};
    RType::Render::Rect viewport = RType::Render::computeViewport(window, VIRTUAL);

    EXPECT_FLOAT_EQ(viewport.x * 2.0F + viewport.width, window.x);
    EXPECT_FLOAT_EQ(viewport.y * 2.0F + viewport.height, window.y);
    EXPECT_NEAR(viewport.width / viewport.height, VIRTUAL.x / VIRTUAL.y, 1e-4F);
}

TEST(render_Letterbox, nonPositiveSizesGiveEmptyRect)
{
    const RType::Render::Rect empty = {};

    EXPECT_EQ(RType::Render::computeViewport({0.0F, 0.0F}, VIRTUAL), empty);
    EXPECT_EQ(RType::Render::computeViewport({0.0F, 720.0F}, VIRTUAL), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 0.0F}, VIRTUAL), empty);
    EXPECT_EQ(RType::Render::computeViewport({-1280.0F, 720.0F}, VIRTUAL), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, -720.0F}, VIRTUAL), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, {0.0F, 1080.0F}), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, {1920.0F, 0.0F}), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, {-1920.0F, 1080.0F}), empty);
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, {1920.0F, -1080.0F}), empty);
}

// NaN is not strictly positive, so the header promises an empty Rect
TEST(render_Letterbox, nanSizeGivesEmptyRect)
{
    const float nan = std::numeric_limits<float>::quiet_NaN();

    EXPECT_EQ(RType::Render::computeViewport({nan, 720.0F}, VIRTUAL), RType::Render::Rect{});
    EXPECT_EQ(RType::Render::computeViewport({1280.0F, 720.0F}, {nan, 1080.0F}), RType::Render::Rect{});
}


/////////////////////////////////////////////////////////
//                                                     //
//                   screenToVirtual                   //
//                                                     //
/////////////////////////////////////////////////////////

TEST(render_Letterbox, viewportCornersMapToVirtualCorners)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    expectNear(RType::Render::screenToVirtual({viewport.x, viewport.y}, viewport, VIRTUAL), {0.0F, 0.0F});
    expectNear(RType::Render::screenToVirtual({viewport.x + viewport.width, viewport.y + viewport.height}, viewport, VIRTUAL),
               VIRTUAL);
    expectNear(RType::Render::screenToVirtual({1280.0F, 540.0F}, viewport, VIRTUAL), {960.0F, 540.0F});
}

TEST(render_Letterbox, scaledViewportMapsToVirtual)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({1280.0F, 720.0F}, VIRTUAL);

    expectNear(RType::Render::screenToVirtual({640.0F, 360.0F}, viewport, VIRTUAL), {960.0F, 540.0F});
    expectNear(RType::Render::screenToVirtual({128.0F, 72.0F}, viewport, VIRTUAL), {192.0F, 108.0F});
}

TEST(render_Letterbox, roundTripFromVirtual)
{
    const RType::Render::Vec2 windows[] = {{1280.0F, 720.0F}, {2560.0F, 1080.0F}, {1920.0F, 1440.0F}, {333.0F, 777.0F}};
    const RType::Render::Vec2 points[] = {{0.0F, 0.0F}, {1920.0F, 1080.0F}, {123.5F, 987.25F}, {1500.0F, 20.0F}};

    for (RType::Render::Vec2 window : windows) {
        RType::Render::Rect viewport = RType::Render::computeViewport(window, VIRTUAL);

        for (RType::Render::Vec2 point : points) {
            RType::Render::Vec2 screen = {
                viewport.x + point.x * viewport.width / VIRTUAL.x,
                viewport.y + point.y * viewport.height / VIRTUAL.y,
            };

            expectNear(RType::Render::screenToVirtual(screen, viewport, VIRTUAL), point);
        }
    }
}

TEST(render_Letterbox, outsideViewportIsNotClamped)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    RType::Render::Vec2 left = RType::Render::screenToVirtual({0.0F, 540.0F}, viewport, VIRTUAL);
    RType::Render::Vec2 right = RType::Render::screenToVirtual({2560.0F, 540.0F}, viewport, VIRTUAL);

    expectNear(left, {-320.0F, 540.0F});
    expectNear(right, {2240.0F, 540.0F});
}

TEST(render_Letterbox, outsideViewportVertically)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({1920.0F, 1440.0F}, VIRTUAL);

    expectNear(RType::Render::screenToVirtual({0.0F, 0.0F}, viewport, VIRTUAL), {0.0F, -180.0F});
    expectNear(RType::Render::screenToVirtual({1920.0F, 1440.0F}, viewport, VIRTUAL), {1920.0F, 1260.0F});
}

TEST(render_Letterbox, emptyViewportGivesOrigin)
{
    const RType::Render::Vec2 origin = {};

    EXPECT_EQ(RType::Render::screenToVirtual({10.0F, 20.0F}, RType::Render::Rect{}, VIRTUAL), origin);
    EXPECT_EQ(RType::Render::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, 0.0F, 100.0F}, VIRTUAL), origin);
    EXPECT_EQ(RType::Render::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, 100.0F, 0.0F}, VIRTUAL), origin);
    EXPECT_EQ(RType::Render::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, -100.0F, 100.0F}, VIRTUAL), origin);
}

TEST(render_Letterbox, minimizedWindowGivesOrigin)
{
    RType::Render::Rect viewport = RType::Render::computeViewport({0.0F, 0.0F}, VIRTUAL);

    EXPECT_EQ(RType::Render::screenToVirtual({100.0F, 100.0F}, viewport, VIRTUAL), RType::Render::Vec2{});
}
