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

    // Round-trip at distance = π/2·R (quarter sphere).
    // At this distance n1 = cos(π/2) ≈ 0, causing catastrophic cancellation in
    // the original a = (n4 - n2·b)/n1 formula: the deg/rad round-trip in to.lat
    // leaves n4 - n2·b ≈ 1 ULP, which gets amplified by 1/n1 ≈ 1.6e16.
    // Two valid origins exist at this distance; verify the returned one actually
    // maps back to 'to' rather than containing the garbage cancellation produces.
    {
        const double half_pi_R = M_PI / 2 * MathUtil::EARTH_RADIUS;
        LatLng to = SphericalUtil::computeOffset(LatLng(30.0, 20.0), half_pi_R, 45.0);
        auto r = SphericalUtil::computeOffsetOrigin(to, half_pi_R, 45.0);
        ASSERT_TRUE(r.has_value());
        EXPECT_NEAR_LatLng(to, SphericalUtil::computeOffset(r.value(), half_pi_R, 45.0));
    }
}
