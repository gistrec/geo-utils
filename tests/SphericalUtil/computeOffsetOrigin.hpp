#include <gtest/gtest.h>

#include "SphericalUtil.hpp"
#include "../TestHelpers.hpp"

TEST(SphericalUtil, computeOffsetOrigin) {
    LatLng front = {  0.0,    0.0 };

    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffsetOrigin(front, 0, 0));

    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffsetOrigin(LatLng(  0,  45), M_PI * MathUtil::EARTH_RADIUS / 4,  90));
    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffsetOrigin(LatLng(  0, -45), M_PI * MathUtil::EARTH_RADIUS / 4, -90));
    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffsetOrigin(LatLng( 45,   0), M_PI * MathUtil::EARTH_RADIUS / 4,   0));
    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffsetOrigin(LatLng(-45,   0), M_PI * MathUtil::EARTH_RADIUS / 4, 180));

    // Issue #3
    // Situations with no solution, should return null.
    //
    // First 'over' the pole.
    // EXPECT_NULL(SphericalUtil::computeOffsetOrigin(LatLng(80, 0), M_PI * MathUtil::EARTH_RADIUS / 4, 180));

    // Second a distance that doesn't fit on the earth.
    // EXPECT_NULL(SphericalUtil::computeOffsetOrigin(LatLng(80, 0), M_PI * MathUtil::EARTH_RADIUS / 4, 90));

    // Longitude regression: computeOffset({0,30}, 45° arc, east) = {0,75},
    // so the inverse must recover lng=30, not just lat.
    {
        LatLng result = SphericalUtil::computeOffsetOrigin(LatLng(0, 75), M_PI * MathUtil::EARTH_RADIUS / 4, 90);
        EXPECT_NEAR(result.lat,  0.0, 1e-6);
        EXPECT_NEAR(result.lng, 30.0, 1e-6);
    }

    // Same check heading west: computeOffset({0,-30}, 45° arc, west) = {0,-75}
    {
        LatLng result = SphericalUtil::computeOffsetOrigin(LatLng(0, -75), M_PI * MathUtil::EARTH_RADIUS / 4, -90);
        EXPECT_NEAR(result.lat,   0.0, 1e-6);
        EXPECT_NEAR(result.lng, -30.0, 1e-6);
    }
}
