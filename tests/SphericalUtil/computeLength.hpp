#include <gtest/gtest.h>
#include <vector>

#include "SphericalUtil.hpp"


TEST(SphericalUtil, computeLength) {
    // List without points
    std::vector<LatLng> latLngs;
    EXPECT_NEAR(SphericalUtil::computeLength(latLngs), 0, 1e-6);
    
    // List with one point
    latLngs.push_back(LatLng(0, 0));
    EXPECT_NEAR(SphericalUtil::computeLength(latLngs), 0, 1e-6);

    // List with two points 
    latLngs.push_back(LatLng(0.1, 0.1));
    EXPECT_NEAR(SphericalUtil::computeLength(latLngs), deg2rad(0.1) * sqrt(2) * MathUtil::EARTH_RADIUS, 1);

    // List with three points
    std::vector<LatLng> latLngs2 = { {0, 0}, {90, 0}, {0, 90} };
    EXPECT_NEAR(SphericalUtil::computeLength(latLngs2), M_PI * MathUtil::EARTH_RADIUS, 1e-6);
}
