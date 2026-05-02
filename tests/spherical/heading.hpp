#include <gtest/gtest.h>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::heading;

TEST(Spherical, heading) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    // Opposing vertices for which there is a result
    EXPECT_NEAR(heading(up, down), -180, 1e-6);
    EXPECT_NEAR(heading(down, up),    0, 1e-6);

    // Adjacent vertices for which there is a result
    EXPECT_NEAR(heading(front, up), 0, 1e-6);
    EXPECT_NEAR(heading(right, up), 0, 1e-6);
    EXPECT_NEAR(heading(back, up),  0, 1e-6);

    EXPECT_NEAR(heading(front, down), -180, 1e-6);
    EXPECT_NEAR(heading(right, down), -180, 1e-6);
    EXPECT_NEAR(heading(back, down),  -180, 1e-6);
    EXPECT_NEAR(heading(left, down),  -180, 1e-6);

    EXPECT_NEAR(heading(right, front), -90, 1e-6);
    EXPECT_NEAR(heading(left, front),   90, 1e-6);

    EXPECT_NEAR(heading(front, right), 90, 1e-6);
    EXPECT_NEAR(heading(back, right), -90, 1e-6);
}
