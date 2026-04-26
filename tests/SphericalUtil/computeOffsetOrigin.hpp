#include <gtest/gtest.h>

#include "SphericalUtil.hpp"
#include "../TestHelpers.hpp"

TEST(SphericalUtil, computeOffsetOrigin) {
    LatLng front = {  0.0,    0.0 };

    // Zero distance: origin equals destination
    {
        auto r = SphericalUtil::computeOffsetOrigin(front, 0, 0);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(front, r.value());
    }

    // Valid cardinal recoveries
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng(  0,  45), M_PI * MathUtil::EARTH_RADIUS / 4,  90);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(front, r.value());
    }
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng(  0, -45), M_PI * MathUtil::EARTH_RADIUS / 4, -90);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(front, r.value());
    }
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng( 45,   0), M_PI * MathUtil::EARTH_RADIUS / 4,   0);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(front, r.value());
    }
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng(-45,   0), M_PI * MathUtil::EARTH_RADIUS / 4, 180);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(front, r.value());
    }

    // No-solution cases (Issue #3): destination unreachable with given distance and heading.
    // heading=180 (south): origin would need lat > 90°.
    EXPECT_FALSE(SphericalUtil::computeOffsetOrigin(LatLng(80, 0), M_PI * MathUtil::EARTH_RADIUS / 4, 180).has_value());
    // heading=90 (east): moving east never changes latitude, so (80°,0°) is unreachable.
    EXPECT_FALSE(SphericalUtil::computeOffsetOrigin(LatLng(80, 0), M_PI * MathUtil::EARTH_RADIUS / 4,  90).has_value());

    // Longitude regression: computeOffset({0,30}, 45° arc, east) = {0,75},
    // so the inverse must recover lng=30, not just lat.
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng(0, 75), M_PI * MathUtil::EARTH_RADIUS / 4, 90);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR(r->lat,  0.0, 1e-6);
        EXPECT_NEAR(r->lng, 30.0, 1e-6);
    }

    // Same check heading west: computeOffset({0,-30}, 45° arc, west) = {0,-75}
    {
        auto r = SphericalUtil::computeOffsetOrigin(LatLng(0, -75), M_PI * MathUtil::EARTH_RADIUS / 4, -90);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR(r->lat,   0.0, 1e-6);
        EXPECT_NEAR(r->lng, -30.0, 1e-6);
    }
}
