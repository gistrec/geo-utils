#include <gtest/gtest.h>

#include <CppGeometryLibrary/MathUtil.hpp>


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

TEST(MathUtil, mercator_inverseMercator) {
    // Equator maps to 0
    EXPECT_NEAR(MathUtil::mercator(0.0), 0.0, 1e-10);
    EXPECT_NEAR(MathUtil::inverseMercator(0.0), 0.0, 1e-10);

    // Round-trip: inverseMercator(mercator(lat)) == lat
    EXPECT_NEAR(MathUtil::inverseMercator(MathUtil::mercator(deg2rad( 45.0))), deg2rad( 45.0), 1e-10);
    EXPECT_NEAR(MathUtil::inverseMercator(MathUtil::mercator(deg2rad(-30.0))), deg2rad(-30.0), 1e-10);
}

TEST(MathUtil, hav_arcHav) {
    EXPECT_NEAR(MathUtil::hav(0),        0.0, 1e-10);
    EXPECT_NEAR(MathUtil::hav(M_PI / 2), 0.5, 1e-10);
    EXPECT_NEAR(MathUtil::hav(M_PI),     1.0, 1e-10);

    EXPECT_NEAR(MathUtil::arcHav(0),   0.0,     1e-10);
    EXPECT_NEAR(MathUtil::arcHav(0.5), M_PI / 2, 1e-10);
    EXPECT_NEAR(MathUtil::arcHav(1),   M_PI,    1e-10);

    // Round-trip: arcHav(hav(x)) == x for x in [0, π]
    EXPECT_NEAR(MathUtil::arcHav(MathUtil::hav(1.2)), 1.2, 1e-10);
}

TEST(MathUtil, arcHav_clamp) {
    // Floating-point rounding can push the haversine value slightly outside [0,1].
    // arcHav must clamp rather than pass a bad value to sqrt/asin and return NaN.
    EXPECT_NEAR(MathUtil::arcHav( 1.0 + 1e-15),  M_PI, 1e-9); // x > 1 → clamp to π
    EXPECT_NEAR(MathUtil::arcHav(-1e-15),         0.0, 1e-9); // x < 0 → clamp to 0
}

TEST(MathUtil, sinFromHav) {
    EXPECT_NEAR(MathUtil::sinFromHav(0.0), 0.0, 1e-10); // sin(0) == 0
    EXPECT_NEAR(MathUtil::sinFromHav(0.5), 1.0, 1e-10); // sin(π/2) == 1
    EXPECT_NEAR(MathUtil::sinFromHav(1.0), 0.0, 1e-10); // sin(π) == 0
}

TEST(MathUtil, havFromSin) {
    EXPECT_NEAR(MathUtil::havFromSin(0.0), 0.0, 1e-10); // hav(asin(0)) == 0
    EXPECT_NEAR(MathUtil::havFromSin(1.0), 0.5, 1e-10); // hav(π/2) == 0.5

    // Round-trip: sinFromHav(havFromSin(x)) == x for x in [0, 1]
    EXPECT_NEAR(MathUtil::sinFromHav(MathUtil::havFromSin(0.7)), 0.7, 1e-10);
}

TEST(MathUtil, sinSumFromHav) {
    // sin(0 + 0) == 0
    EXPECT_NEAR(MathUtil::sinSumFromHav(0.0, 0.0), 0.0, 1e-10);
    // sin(π/2 + 0) == 1
    EXPECT_NEAR(MathUtil::sinSumFromHav(0.5, 0.0), 1.0, 1e-10);
    // sin(π/3 + π/3) == sin(2π/3) == √3/2
    EXPECT_NEAR(MathUtil::sinSumFromHav(0.25, 0.25), sqrt(3.0) / 2.0, 1e-10);
}

TEST(MathUtil, havDistance) {
    // Distance from a point to itself is 0
    EXPECT_NEAR(MathUtil::havDistance(0, 0, 0), 0.0, 1e-10);

    // front(0,0) → right(0,90°): dLng = π/2, hav(0) + hav(π/2)*1*1 = 0.5
    EXPECT_NEAR(MathUtil::havDistance(0, 0, M_PI / 2), 0.5, 1e-10);

    // front(0,0) → up(90°,0): hav(π/2) + 0 = 0.5
    EXPECT_NEAR(MathUtil::havDistance(0, M_PI / 2, 0), 0.5, 1e-10);
}
