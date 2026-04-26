#include <gtest/gtest.h>

#include <CppGeometryLibrary/LatLng.hpp>

TEST(LatLng, operator_equal) {
    // Basic equality and inequality
    EXPECT_TRUE (LatLng( 0,  0) == LatLng( 0,  0));
    EXPECT_FALSE(LatLng( 0,  0) == LatLng( 1,  0));
    EXPECT_FALSE(LatLng( 0,  0) == LatLng( 0,  1));
    EXPECT_FALSE(LatLng( 1,  0) == LatLng(-1,  0));

    // ±180° is the same meridian
    EXPECT_TRUE(LatLng(  0,  180) == LatLng(  0, -180));
    EXPECT_TRUE(LatLng( 45,  180) == LatLng( 45, -180));
    EXPECT_TRUE(LatLng(-45,  180) == LatLng(-45, -180));
}
