#include <gtest/gtest.h>

#include "SphericalUtil.hpp"
#include "../TestHelpers.hpp"

TEST(SphericalUtil, computeOffset) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffset(front, 0, 0));
    EXPECT_NEAR_LatLng(up,    SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,   0));
    EXPECT_NEAR_LatLng(down,  SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, 180));
    EXPECT_NEAR_LatLng(left,  SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, -90));
    EXPECT_NEAR_LatLng(right, SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,  90));
    EXPECT_NEAR_LatLng(back,  SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS,       0));
    EXPECT_NEAR_LatLng(back,  SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS,      90));

    // From left
    EXPECT_NEAR_LatLng(left,  SphericalUtil::computeOffset(left, 0, 0));
    EXPECT_NEAR_LatLng(up,    SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS / 2,   0));
    EXPECT_NEAR_LatLng(down,  SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS / 2, 180));
    EXPECT_NEAR_LatLng(front, SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS / 2,  90));
    EXPECT_NEAR_LatLng(back,  SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS / 2, -90));
    EXPECT_NEAR_LatLng(right, SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS,       0));
    EXPECT_NEAR_LatLng(right, SphericalUtil::computeOffset(left, M_PI * MathUtil::EARTH_RADIUS,      90));

    // NOTE: Heading is undefined at the poles, so we do not test from up/down.
}
