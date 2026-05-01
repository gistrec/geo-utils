#include <gtest/gtest.h>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::distance_between;
using geo::angle_between;
using geo::detail::kEarthRadius;
using geo::detail::kPi;

TEST(Spherical, distance_between) {
    LatLng up    = { 90.0,  0.0 };
    LatLng down  = {-90.0,  0.0 };
    LatLng front = {  0.0,  0.0 };
    LatLng right = {  0.0, 90.0 };

    // Antipodal points
    EXPECT_NEAR(distance_between(up, down), kPi * kEarthRadius, 1e-6);

    // Same point
    EXPECT_NEAR(distance_between(front, front), 0.0, 1e-6);
    EXPECT_NEAR(distance_between(up, up),       0.0, 1e-6);

    // Quarter arc (90° apart)
    EXPECT_NEAR(distance_between(front, up),    kPi / 2 * kEarthRadius, 1e-6);
    EXPECT_NEAR(distance_between(front, right), kPi / 2 * kEarthRadius, 1e-6);

    // Consistent with angle_between
    EXPECT_NEAR(distance_between(front, up),
                angle_between(front, up) * kEarthRadius, 1e-6);
}
