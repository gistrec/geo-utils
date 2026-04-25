#include <gtest/gtest.h>

#include "SphericalUtil.hpp"


TEST(SphericalUtil, computeDistanceBetween) {
    LatLng up    = { 90.0,  0.0 };
    LatLng down  = {-90.0,  0.0 };
    LatLng front = {  0.0,  0.0 };
    LatLng right = {  0.0, 90.0 };

    // Antipodal points
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(up, down), M_PI * MathUtil::EARTH_RADIUS, 1e-6);

    // Same point
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(front, front), 0.0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(up, up),       0.0, 1e-6);

    // Quarter arc (90° apart)
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(front, up),    M_PI / 2 * MathUtil::EARTH_RADIUS, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(front, right), M_PI / 2 * MathUtil::EARTH_RADIUS, 1e-6);

    // Consistent with computeAngleBetween
    EXPECT_NEAR(SphericalUtil::computeDistanceBetween(front, up),
                SphericalUtil::computeAngleBetween(front, up) * MathUtil::EARTH_RADIUS, 1e-6);
}
