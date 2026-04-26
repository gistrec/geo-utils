#include <gtest/gtest.h>
#include <vector>

#include <CppGeometryLibrary/PolyUtil.hpp>


TEST(PolyUtil, containsLocation) {
    // Empty.
    std::vector<LatLng> empty;
    EXPECT_FALSE(PolyUtil::containsLocation(LatLng(0, 0), empty,  true));
    EXPECT_FALSE(PolyUtil::containsLocation(LatLng(0, 0), empty, false));


    // One point.
    std::vector<LatLng> one = { {1, 2} };
    EXPECT_TRUE(PolyUtil::containsLocation(LatLng(1, 2), one,  true));
    EXPECT_TRUE(PolyUtil::containsLocation(LatLng(1, 2), one, false));

    EXPECT_FALSE(PolyUtil::containsLocation(LatLng(0, 0), one,  true));
    EXPECT_FALSE(PolyUtil::containsLocation(LatLng(0, 0), one, false));


    // Two points.
    std::vector<LatLng> two = { {1, 2}, {3, 5} };
    for (const auto & point : { LatLng(1, 2), LatLng(3, 5) }) {
        EXPECT_TRUE(PolyUtil::containsLocation(point, two,  true));
        EXPECT_TRUE(PolyUtil::containsLocation(point, two, false));
    }
    for (const auto & point : { LatLng(0, 0), LatLng(40, 4) }) {
        EXPECT_FALSE(PolyUtil::containsLocation(point, two,  true));
        EXPECT_FALSE(PolyUtil::containsLocation(point, two, false));
    }
    

    // Some arbitrary triangle.
    std::vector<LatLng> triangle = { {0, 0}, {10, 12}, {20, 5} };
    for (const auto & point : { LatLng(10, 12), LatLng(10, 11), LatLng(19, 5) }) {
        EXPECT_TRUE(PolyUtil::containsLocation(point, triangle,  true));
        EXPECT_TRUE(PolyUtil::containsLocation(point, triangle, false));
    }
    for (const auto & point : { LatLng(0, 1), LatLng(11, 12), LatLng(30, 5), LatLng(0, -180), LatLng(0, 90) }) {
        EXPECT_FALSE(PolyUtil::containsLocation(point, triangle,  true));
        EXPECT_FALSE(PolyUtil::containsLocation(point, triangle, false));
    }


    // Around North Pole.
    std::vector<LatLng> northPole = { {89, 0}, {89, 120}, {89, -120} };
    for (const auto & point : { LatLng(90, 0), /* LatLng(90, 180), */ LatLng(90, -90) }) {
        EXPECT_TRUE(PolyUtil::containsLocation(point, northPole,  true));
        EXPECT_TRUE(PolyUtil::containsLocation(point, northPole, false));
    }
    for (const auto & point : { LatLng(-90, 0), LatLng(0, 0) }) {
        EXPECT_FALSE(PolyUtil::containsLocation(point, northPole,  true));
        EXPECT_FALSE(PolyUtil::containsLocation(point, northPole, false));
    }

    // Around South Pole.
    std::vector<LatLng> southPole = { {-89, 0}, {-89, 120}, {-89, -120} };
    for (const auto & point : { LatLng(90, 0), /* LatLng(90, 180), */ LatLng(90, -90), LatLng(0, 0) }) {
        EXPECT_TRUE(PolyUtil::containsLocation(point, southPole,  true));
        EXPECT_TRUE(PolyUtil::containsLocation(point, southPole, false));
    }
    for (const auto & point : { LatLng(-90, 0), LatLng(-90, 90) }) {
        EXPECT_FALSE(PolyUtil::containsLocation(point, southPole,  true));
        EXPECT_FALSE(PolyUtil::containsLocation(point, southPole, false));
    }

    // Over/under segment on meridian and equator.
    std::vector<LatLng> poly = { {5, 10}, {10, 10}, {0, 20}, {0, -10} };
    for (const auto & point : { LatLng(2.5, 10), LatLng(1, 0) }) {
        EXPECT_TRUE(PolyUtil::containsLocation(point, poly,  true));
        EXPECT_TRUE(PolyUtil::containsLocation(point, poly, false));
    }
    for (const auto & point : { LatLng(15, 10), LatLng(0, -15), LatLng(0, 25), LatLng(-1, 0) }) {
        EXPECT_FALSE(PolyUtil::containsLocation(point, poly,  true));
        EXPECT_FALSE(PolyUtil::containsLocation(point, poly, false));
    }
}
