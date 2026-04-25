#include <gtest/gtest.h>

#include "MathUtil.hpp"


TEST(MathUtil, mod_positive) {
    EXPECT_NEAR(MathUtil::mod(7.0, 3.0),  1.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(6.0, 3.0),  0.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(0.5, 1.0),  0.5, 1e-10);
}

TEST(MathUtil, mod_negative_x) {
    // Must return non-negative result
    EXPECT_NEAR(MathUtil::mod(-1.0,  3.0), 2.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(-3.0,  3.0), 0.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(-7.0,  3.0), 2.0, 1e-10);
}

TEST(MathUtil, mod_wrap_longitude) {
    // Typical use: wrapping longitude into [0, 360)
    EXPECT_NEAR(MathUtil::mod(360.0, 360.0),   0.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(370.0, 360.0),  10.0, 1e-10);
    EXPECT_NEAR(MathUtil::mod(-10.0, 360.0), 350.0, 1e-10);
}

TEST(MathUtil, wrap) {
    // wrap uses mod internally — verify it returns correct results for negative input
    EXPECT_NEAR(MathUtil::wrap(-181.0, -180.0, 180.0), 179.0, 1e-10);
    EXPECT_NEAR(MathUtil::wrap( 181.0, -180.0, 180.0),-179.0, 1e-10);
    EXPECT_NEAR(MathUtil::wrap(   0.0, -180.0, 180.0),   0.0, 1e-10);
    EXPECT_NEAR(MathUtil::wrap(-180.0, -180.0, 180.0),-180.0, 1e-10);
}

TEST(MathUtil, clamp) {
    EXPECT_NEAR(MathUtil::clamp( 5.0, 0.0, 10.0),  5.0, 1e-10);
    EXPECT_NEAR(MathUtil::clamp(-1.0, 0.0, 10.0),  0.0, 1e-10);
    EXPECT_NEAR(MathUtil::clamp(11.0, 0.0, 10.0), 10.0, 1e-10);
}
