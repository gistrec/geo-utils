#include <gtest/gtest.h>
#include <vector>

#include <CppGeometryLibrary/SphericalUtil.hpp>


TEST(SphericalUtil, computeArea) {
    LatLng up    = { 90.0,  0.0 };
    LatLng down  = {-90.0,  0.0 };
    LatLng front = {  0.0,  0.0 };
    LatLng right = {  0.0, 90.0 };

    std::vector<LatLng> first = { right, up, front, down, right };
    EXPECT_NEAR(SphericalUtil::computeArea(first), M_PI * MathUtil::EARTH_RADIUS * MathUtil::EARTH_RADIUS, .4);

    std::vector<LatLng> second = { right, down, front, up, right };
    EXPECT_NEAR(SphericalUtil::computeArea(second), M_PI * MathUtil::EARTH_RADIUS * MathUtil::EARTH_RADIUS, .4);
}
