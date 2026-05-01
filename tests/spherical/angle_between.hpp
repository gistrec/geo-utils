#include <gtest/gtest.h>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::angle_between;
using geo::detail::kPi;

TEST(Spherical, angle_between) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    EXPECT_NEAR(angle_between(up, up),       0, 1e-6);
    EXPECT_NEAR(angle_between(down, down),   0, 1e-6);
    EXPECT_NEAR(angle_between(left, left),   0, 1e-6);
    EXPECT_NEAR(angle_between(right, right), 0, 1e-6);
    EXPECT_NEAR(angle_between(front, front), 0, 1e-6);
    EXPECT_NEAR(angle_between(back, back),   0, 1e-6);

    // Adjacent vertices
    EXPECT_NEAR(angle_between(up, front), kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(up, right), kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(up, back),  kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(up, left),  kPi / 2, 1e-6);

    EXPECT_NEAR(angle_between(down, front), kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(down, right), kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(down, back),  kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(down, left),  kPi / 2, 1e-6);

    EXPECT_NEAR(angle_between(back, up),    kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(back, right), kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(back, down),  kPi / 2, 1e-6);
    EXPECT_NEAR(angle_between(back, left),  kPi / 2, 1e-6);

    // Opposite vertices
    EXPECT_NEAR(angle_between(up, down),    kPi, 1e-6);
    EXPECT_NEAR(angle_between(front, back), kPi, 1e-6);
    EXPECT_NEAR(angle_between(left, right), kPi, 1e-6);
}
