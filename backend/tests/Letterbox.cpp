/*
** EPITECH PROJECT, 2026
** Project - R-Type
** File description:
** Unit tests for
** letterbox math
*/

#include <gtest/gtest.h>
#include <backend/Letterbox.hpp>
#include <limits>

namespace {
    constexpr Backend::Vec2 VIRTUAL = {1920.0F, 1080.0F};

    void expectNear(Backend::Vec2 actual, Backend::Vec2 expected)
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

TEST(backend_Letterbox, sameSizeFillsWindow)
{
    EXPECT_EQ(Backend::computeViewport(VIRTUAL, VIRTUAL), (Backend::Rect{0.0F, 0.0F, 1920.0F, 1080.0F}));
}

TEST(backend_Letterbox, sameAspectScalesDown)
{
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, VIRTUAL), (Backend::Rect{0.0F, 0.0F, 1280.0F, 720.0F}));
}

TEST(backend_Letterbox, sameAspectScalesUp)
{
    EXPECT_EQ(Backend::computeViewport({3840.0F, 2160.0F}, VIRTUAL), (Backend::Rect{0.0F, 0.0F, 3840.0F, 2160.0F}));
}

TEST(backend_Letterbox, widerWindowGetsSideBars)
{
    Backend::Rect viewport = Backend::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    EXPECT_EQ(viewport, (Backend::Rect{320.0F, 0.0F, 1920.0F, 1080.0F}));
}

TEST(backend_Letterbox, tallerWindowGetsTopBottomBars)
{
    Backend::Rect viewport = Backend::computeViewport({1920.0F, 1440.0F}, VIRTUAL);

    EXPECT_EQ(viewport, (Backend::Rect{0.0F, 180.0F, 1920.0F, 1080.0F}));
}

TEST(backend_Letterbox, squareWindowKeepsAspect)
{
    Backend::Rect viewport = Backend::computeViewport({800.0F, 800.0F}, VIRTUAL);

    EXPECT_FLOAT_EQ(viewport.x, 0.0F);
    EXPECT_FLOAT_EQ(viewport.width, 800.0F);
    EXPECT_FLOAT_EQ(viewport.height, 450.0F);
    EXPECT_FLOAT_EQ(viewport.y, 175.0F);
}

TEST(backend_Letterbox, viewportIsCentered)
{
    Backend::Vec2 window = {1000.0F, 300.0F};
    Backend::Rect viewport = Backend::computeViewport(window, VIRTUAL);

    EXPECT_FLOAT_EQ(viewport.x * 2.0F + viewport.width, window.x);
    EXPECT_FLOAT_EQ(viewport.y * 2.0F + viewport.height, window.y);
    EXPECT_NEAR(viewport.width / viewport.height, VIRTUAL.x / VIRTUAL.y, 1e-4F);
}

TEST(backend_Letterbox, nonPositiveSizesGiveEmptyRect)
{
    const Backend::Rect empty = {};

    EXPECT_EQ(Backend::computeViewport({0.0F, 0.0F}, VIRTUAL), empty);
    EXPECT_EQ(Backend::computeViewport({0.0F, 720.0F}, VIRTUAL), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, 0.0F}, VIRTUAL), empty);
    EXPECT_EQ(Backend::computeViewport({-1280.0F, 720.0F}, VIRTUAL), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, -720.0F}, VIRTUAL), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, {0.0F, 1080.0F}), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, {1920.0F, 0.0F}), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, {-1920.0F, 1080.0F}), empty);
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, {1920.0F, -1080.0F}), empty);
}

// NaN is not strictly positive, so the header promises an empty Rect
TEST(backend_Letterbox, nanSizeGivesEmptyRect)
{
    const float nan = std::numeric_limits<float>::quiet_NaN();

    EXPECT_EQ(Backend::computeViewport({nan, 720.0F}, VIRTUAL), Backend::Rect{});
    EXPECT_EQ(Backend::computeViewport({1280.0F, 720.0F}, {nan, 1080.0F}), Backend::Rect{});
}


/////////////////////////////////////////////////////////
//                                                     //
//                   screenToVirtual                   //
//                                                     //
/////////////////////////////////////////////////////////

TEST(backend_Letterbox, viewportCornersMapToVirtualCorners)
{
    Backend::Rect viewport = Backend::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    expectNear(Backend::screenToVirtual({viewport.x, viewport.y}, viewport, VIRTUAL), {0.0F, 0.0F});
    expectNear(Backend::screenToVirtual({viewport.x + viewport.width, viewport.y + viewport.height}, viewport, VIRTUAL),
               VIRTUAL);
    expectNear(Backend::screenToVirtual({1280.0F, 540.0F}, viewport, VIRTUAL), {960.0F, 540.0F});
}

TEST(backend_Letterbox, scaledViewportMapsToVirtual)
{
    Backend::Rect viewport = Backend::computeViewport({1280.0F, 720.0F}, VIRTUAL);

    expectNear(Backend::screenToVirtual({640.0F, 360.0F}, viewport, VIRTUAL), {960.0F, 540.0F});
    expectNear(Backend::screenToVirtual({128.0F, 72.0F}, viewport, VIRTUAL), {192.0F, 108.0F});
}

TEST(backend_Letterbox, roundTripFromVirtual)
{
    const Backend::Vec2 windows[] = {{1280.0F, 720.0F}, {2560.0F, 1080.0F}, {1920.0F, 1440.0F}, {333.0F, 777.0F}};
    const Backend::Vec2 points[] = {{0.0F, 0.0F}, {1920.0F, 1080.0F}, {123.5F, 987.25F}, {1500.0F, 20.0F}};

    for (Backend::Vec2 window : windows) {
        Backend::Rect viewport = Backend::computeViewport(window, VIRTUAL);

        for (Backend::Vec2 point : points) {
            Backend::Vec2 screen = {
                viewport.x + point.x * viewport.width / VIRTUAL.x,
                viewport.y + point.y * viewport.height / VIRTUAL.y,
            };

            expectNear(Backend::screenToVirtual(screen, viewport, VIRTUAL), point);
        }
    }
}

TEST(backend_Letterbox, outsideViewportIsNotClamped)
{
    Backend::Rect viewport = Backend::computeViewport({2560.0F, 1080.0F}, VIRTUAL);

    Backend::Vec2 left = Backend::screenToVirtual({0.0F, 540.0F}, viewport, VIRTUAL);
    Backend::Vec2 right = Backend::screenToVirtual({2560.0F, 540.0F}, viewport, VIRTUAL);

    expectNear(left, {-320.0F, 540.0F});
    expectNear(right, {2240.0F, 540.0F});
}

TEST(backend_Letterbox, outsideViewportVertically)
{
    Backend::Rect viewport = Backend::computeViewport({1920.0F, 1440.0F}, VIRTUAL);

    expectNear(Backend::screenToVirtual({0.0F, 0.0F}, viewport, VIRTUAL), {0.0F, -180.0F});
    expectNear(Backend::screenToVirtual({1920.0F, 1440.0F}, viewport, VIRTUAL), {1920.0F, 1260.0F});
}

TEST(backend_Letterbox, emptyViewportGivesOrigin)
{
    const Backend::Vec2 origin = {};

    EXPECT_EQ(Backend::screenToVirtual({10.0F, 20.0F}, Backend::Rect{}, VIRTUAL), origin);
    EXPECT_EQ(Backend::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, 0.0F, 100.0F}, VIRTUAL), origin);
    EXPECT_EQ(Backend::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, 100.0F, 0.0F}, VIRTUAL), origin);
    EXPECT_EQ(Backend::screenToVirtual({10.0F, 20.0F}, {5.0F, 5.0F, -100.0F, 100.0F}, VIRTUAL), origin);
}

TEST(backend_Letterbox, minimizedWindowGivesOrigin)
{
    Backend::Rect viewport = Backend::computeViewport({0.0F, 0.0F}, VIRTUAL);

    EXPECT_EQ(Backend::screenToVirtual({100.0F, 100.0F}, viewport, VIRTUAL), Backend::Vec2{});
}
