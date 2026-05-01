#include <gtest/gtest.h>

#include <geo/latlng.hpp>

using geo::LatLng;

// Constructor must be constexpr — guarantees usability in constant-evaluation
// contexts (e.g. as a non-type template argument's component, or in
// `inline constexpr LatLng` constants).
static_assert(LatLng(1.0, 2.0).lat == 1.0);
static_assert(LatLng(1.0, 2.0).lng == 2.0);

TEST(LatLng, operator_equal) {
    // Basic equality and inequality
    EXPECT_TRUE (LatLng( 0, 0) == LatLng( 0, 0));
    EXPECT_FALSE(LatLng( 0, 0) == LatLng( 1, 0));
    EXPECT_FALSE(LatLng( 0, 0) == LatLng( 0, 1));
    EXPECT_FALSE(LatLng( 1, 0) == LatLng(-1, 0));

    // ±180° is the same meridian
    EXPECT_TRUE(LatLng(  0, 180) == LatLng(  0, -180));
    EXPECT_TRUE(LatLng( 45, 180) == LatLng( 45, -180));
    EXPECT_TRUE(LatLng(-45, 180) == LatLng(-45, -180));

    // operator!= mirrors operator==
    EXPECT_TRUE (LatLng(0, 0) != LatLng(1, 0));
    EXPECT_FALSE(LatLng(0, 180) != LatLng(0, -180));
}

TEST(LatLng, approx_equal) {
    // Custom tolerance: meter-scale comparison (1m on equator ≈ 9e-6 degrees)
    EXPECT_TRUE (LatLng(40.0, -74.0).approx_equal(LatLng(40.000001, -74.000001), 1e-5));
    EXPECT_FALSE(LatLng(40.0, -74.0).approx_equal(LatLng(40.0001,   -74.0),      1e-5));

    // Default tolerance behaves like operator==
    EXPECT_TRUE (LatLng(0, 180).approx_equal(LatLng(0, -180)));
    EXPECT_FALSE(LatLng(0, 0).approx_equal(LatLng(0, 1e-10)));

    // Tight tolerance rejects what default accepts
    EXPECT_TRUE (LatLng(0, 0).approx_equal(LatLng(0, 1e-13)));
    EXPECT_FALSE(LatLng(0, 0).approx_equal(LatLng(0, 1e-13), 1e-15));

    // Antimeridian wrap is honoured for any tolerance:
    // points near +180 and near -180 must compare equal at coarse tolerances.
    EXPECT_TRUE (LatLng( 45, 179.999).approx_equal(LatLng( 45, -179.999), 1e-2));
    EXPECT_TRUE (LatLng(-45, 179.999).approx_equal(LatLng(-45, -179.999), 1e-2));
    // ...but the lat component still has to match.
    EXPECT_FALSE(LatLng( 45, 179.999).approx_equal(LatLng( 46, -179.999), 1e-2));
}
