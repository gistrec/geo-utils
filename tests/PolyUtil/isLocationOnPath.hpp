#include <gtest/gtest.h>
#include <vector>

#include "PolyUtil.hpp"


TEST(PolyUtil, isLocationOnPath) {
    // Empty
    std::vector<LatLng> empty;
    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(0, 0), empty,  true));
    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(0, 0), empty, false));

    // One point.
    std::vector<LatLng> one = { {1, 2} };
    EXPECT_TRUE(PolyUtil::isLocationOnPath(LatLng(1, 2), one,  true));
    EXPECT_TRUE(PolyUtil::isLocationOnPath(LatLng(1, 2), one, false));

    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(3, 5), one,  true));
    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(3, 5), one, false));

    // Endpoints
    std::vector<LatLng> endpoints = { {1, 2}, {3, 5} };
    for (const auto & point : { LatLng(1, 2), LatLng(3, 5) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, endpoints,  true));
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, endpoints, false));
    }
    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(0, 0), endpoints,  true));
    EXPECT_FALSE(PolyUtil::isLocationOnPath(LatLng(0, 0), endpoints, false));

    double small = 5e-7; // About 5cm on equator, half the default tolerance.
    double big   = 2e-6; // About 10cm on equator, double the default tolerance.

    // On equator.
    std::vector<LatLng> equator = { {0, 90}, {0, 180} };
    for (const auto & point : { LatLng(0, 90-small), LatLng(0, 90+small), LatLng(0-small, 90), LatLng(0, 135), LatLng(small, 135) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, equator,  true));
    }
    for (const auto & point : { LatLng(0, 90 - big), LatLng(0, 0), LatLng(0, -90), LatLng(big, 135) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, equator, false));
    }
    
    // Ends on same latitude.
    std::vector<LatLng> sameLatitude = { {-45, -180}, {-45, -small} };
    for (const auto & point : { LatLng(-45, 180+small), LatLng(-45, 180-small), LatLng(-45-small, 180-small), LatLng(-45, 0) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, sameLatitude,  true));
    }
    for (const auto & point : { LatLng(-45, big), LatLng(-45, 180-big), LatLng(-45+big, -90), LatLng(-45, 90) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, sameLatitude, false));
    }

    // Meridian.
    std::vector<LatLng> meridian = { {-10, 30}, {45, 30} };
    for (const auto & point : { LatLng(10, 30 - small), LatLng(20, 30 + small), LatLng(-10 - small, 30 + small) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, meridian,  true));
    }
    for (const auto & point : { LatLng(-10 - big, 30), LatLng(10, -150), LatLng(0, 30 - big) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, meridian, false));
    }

    // Slanted close to meridian, close to North pole.
    std::vector<LatLng> northPole = { {0, 0}, {90 - small, 0 + big} };
    for (const auto & point : { LatLng(1, 0 + small), LatLng(2, 0 - small), LatLng(90 - small, -90), LatLng(90 - small, 10) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, northPole, true));
    }
    for (const auto & point : { LatLng(-big, 0), LatLng(90 - big, 180), LatLng(10, big) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, northPole, false));
    }

    // Arc > 120 deg.
    std::vector<LatLng> poly = { {0, 0}, {0, 179.999} };
    for (const auto & point : { LatLng(0, 90), LatLng(0, small), LatLng(0, 179), LatLng(small, 90) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, poly, true));
    }
    for (const auto & point : { LatLng(0, -90), LatLng(small, -100), LatLng(0, 180), LatLng(0, -big), LatLng(90, 0), LatLng(-90, 180) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, poly, false));
    }

    std::vector<LatLng> poly2 = { {10, 5}, {30, 15} };
    for (const auto & point : { LatLng(10+2*big, 5+big), LatLng(10+big, 5+big/2), LatLng(30-2*big, 15-big) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, poly2, true));
    }
    for (const auto & point : { LatLng(20, 10), LatLng(10-big, 5-big/2), LatLng(30+2*big, 15+big), LatLng(10+2*big, 5), LatLng(10, 5+big) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, poly2, false));
    }

    std::vector<LatLng> poly3 = { {90 - small, 0}, {0, 180 - small / 2} };
    for (const auto & point : { LatLng(big, -180 + small / 2), LatLng(big, 180 - small / 4), LatLng(big, 180 - small) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, poly3, true));
    }
    for (const auto & point : { LatLng(-big, -180 + small / 2), LatLng(-big, 180), LatLng(-big, 180 - small) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, poly3, false));
    }

    // Reaching close to North pole.
    std::vector<LatLng> closeToNorthPole = { {80, 0}, {80, 180 - small} };

    for (const auto & point : { LatLng(90 - small, -90), LatLng(90, -135), LatLng(80 - small, 0), LatLng(80 + small, 0) }) {
        EXPECT_TRUE(PolyUtil::isLocationOnPath(point, closeToNorthPole, true));
    }
    for (const auto & point : { LatLng(80, 90), LatLng(79, big) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, closeToNorthPole, true));
    }

    for (const auto & point : { LatLng(79, big), LatLng(90 - small, -90), LatLng(90, -135) }) {
        EXPECT_FALSE(PolyUtil::isLocationOnPath(point, closeToNorthPole, false));
    }
}
