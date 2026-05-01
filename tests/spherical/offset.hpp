#include <gtest/gtest.h>

#include <geo/spherical.hpp>
#include "../test_helpers.hpp"

using geo::LatLng;
using geo::offset;
using geo::detail::kEarthRadius;
using geo::detail::kPi;

TEST(Spherical, offset) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    EXPECT_NEAR_LatLng(front, offset(front, 0, 0));
    EXPECT_NEAR_LatLng(up,    offset(front, kPi * kEarthRadius / 2,   0));
    EXPECT_NEAR_LatLng(down,  offset(front, kPi * kEarthRadius / 2, 180));
    EXPECT_NEAR_LatLng(left,  offset(front, kPi * kEarthRadius / 2, -90));
    EXPECT_NEAR_LatLng(right, offset(front, kPi * kEarthRadius / 2,  90));
    EXPECT_NEAR_LatLng(back,  offset(front, kPi * kEarthRadius,       0));
    EXPECT_NEAR_LatLng(back,  offset(front, kPi * kEarthRadius,      90));

    // From left
    EXPECT_NEAR_LatLng(left,  offset(left, 0, 0));
    EXPECT_NEAR_LatLng(up,    offset(left, kPi * kEarthRadius / 2,   0));
    EXPECT_NEAR_LatLng(down,  offset(left, kPi * kEarthRadius / 2, 180));
    EXPECT_NEAR_LatLng(front, offset(left, kPi * kEarthRadius / 2,  90));
    EXPECT_NEAR_LatLng(back,  offset(left, kPi * kEarthRadius / 2, -90));
    EXPECT_NEAR_LatLng(right, offset(left, kPi * kEarthRadius,       0));
    EXPECT_NEAR_LatLng(right, offset(left, kPi * kEarthRadius,      90));

    // NOTE: Heading is undefined at the poles, so we do not test from up/down.
}
