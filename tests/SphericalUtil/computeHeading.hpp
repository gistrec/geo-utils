#include <gtest/gtest.h>

#include "SphericalUtil.hpp"


TEST(SphericalUtil, computeHeading) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    // Opposing vertices for which there is a result
    EXPECT_NEAR(SphericalUtil::computeHeading(up, down), -180, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(down, up),    0, 1e-6);

    // Adjacent vertices for which there is a result
    EXPECT_NEAR(SphericalUtil::computeHeading(front, up), 0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(right, up), 0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(back, up),  0, 1e-6);

    EXPECT_NEAR(SphericalUtil::computeHeading(front, down), -180, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(right, down), -180, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(back, down),  -180, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(left, down),  -180, 1e-6);

    EXPECT_NEAR(SphericalUtil::computeHeading(right, front), -90, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(left, front),   90, 1e-6);

    EXPECT_NEAR(SphericalUtil::computeHeading(front, right), 90, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeHeading(back, right), -90, 1e-6);
}
