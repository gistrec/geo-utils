#include <gtest/gtest.h>
#include <vector>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::signed_area;

TEST(Spherical, signed_area) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };

    std::vector<LatLng> path         = { right,   up, front, down, right };
    std::vector<LatLng> pathReversed = { right, down, front,   up, right };

    EXPECT_NEAR(-signed_area(path), signed_area(pathReversed), 1.0);
}

TEST(Spherical, signed_area_orientation) {
    // Small triangle in the northern hemisphere. Going (0,0) → (0,10) → (10,0)
    // is counter-clockwise when viewed from above the North Pole, and must
    // therefore yield a positive signed area; the reversed path is clockwise
    // and must yield a negative signed area.
    std::vector<LatLng> ccw = { {0, 0}, {0, 10}, {10, 0}, {0, 0} };
    std::vector<LatLng> cw  = { {0, 0}, {10, 0}, {0, 10}, {0, 0} };

    EXPECT_GT(signed_area(ccw), 0.0);
    EXPECT_LT(signed_area(cw),  0.0);
    EXPECT_NEAR(signed_area(ccw), -signed_area(cw), 1.0);
}
