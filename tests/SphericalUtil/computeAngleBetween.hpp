#include <gtest/gtest.h>

#include <CppGeometryLibrary/SphericalUtil.hpp>


TEST(SphericalUtil, computeAngleBetween) {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };
    LatLng back  = {  0.0, -180.0 };
    LatLng left  = {  0.0,  -90.0 };

    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, up),       0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(down, down),   0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(left, left),   0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(right, right), 0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(front, front), 0, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(back, back),   0, 1e-6);

    // Adjacent vertices
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, front), M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, right), M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, back),  M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, left),  M_PI / 2, 1e-6);

    EXPECT_NEAR(SphericalUtil::computeAngleBetween(down, front), M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(down, right), M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(down, back),  M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(down, left),  M_PI / 2, 1e-6);

    EXPECT_NEAR(SphericalUtil::computeAngleBetween(back, up),    M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(back, right), M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(back, down),  M_PI / 2, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(back, left),  M_PI / 2, 1e-6);

    // Opposite vertices
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(up, down),    M_PI, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(front, back), M_PI, 1e-6);
    EXPECT_NEAR(SphericalUtil::computeAngleBetween(left, right), M_PI, 1e-6);
}
