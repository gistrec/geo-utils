#include <gtest/gtest.h>
#include <vector>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::area;
using geo::detail::kEarthRadius;
using geo::detail::kPi;

TEST(Spherical, area) {
    LatLng up    = { 90.0,  0.0 };
    LatLng down  = {-90.0,  0.0 };
    LatLng front = {  0.0,  0.0 };
    LatLng right = {  0.0, 90.0 };

    std::vector<LatLng> first = { right, up, front, down, right };
    EXPECT_NEAR(area(first), kPi * kEarthRadius * kEarthRadius, .4);

    std::vector<LatLng> second = { right, down, front, up, right };
    EXPECT_NEAR(area(second), kPi * kEarthRadius * kEarthRadius, .4);
}
