#include <gtest/gtest.h>
#include <vector>

#include <geo/spherical.hpp>

using geo::LatLng;
using geo::path_length;
using geo::detail::deg2rad;
using geo::detail::kEarthRadius;
using geo::detail::kPi;

TEST(Spherical, path_length) {
    // List without points
    std::vector<LatLng> latLngs;
    EXPECT_NEAR(path_length(latLngs), 0, 1e-6);

    // List with one point
    latLngs.push_back(LatLng(0, 0));
    EXPECT_NEAR(path_length(latLngs), 0, 1e-6);

    // List with two points
    latLngs.push_back(LatLng(0.1, 0.1));
    EXPECT_NEAR(path_length(latLngs), deg2rad(0.1) * sqrt(2) * kEarthRadius, 1);

    // List with three points
    std::vector<LatLng> latLngs2 = { {0, 0}, {90, 0}, {0, 90} };
    EXPECT_NEAR(path_length(latLngs2), kPi * kEarthRadius, 1e-6);
}
