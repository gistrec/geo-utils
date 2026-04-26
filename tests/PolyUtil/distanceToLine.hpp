#include <gtest/gtest.h>
#include <vector>

#include <CppGeometryLibrary/PolyUtil.hpp>
#include <CppGeometryLibrary/SphericalUtil.hpp>


TEST(PolyUtil, distanceToLine) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);
    LatLng point(28.05342, -82.41594);

    double distance = PolyUtil::distanceToLine(point, startLine, endLine);

    EXPECT_NEAR(37.947946, distance, 1e-6);
}

TEST(PolyUtil, distanceToLine_LessThanDistanceToExtrems) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);
    LatLng point(28.05342, -82.41594);

    double distance        = PolyUtil::distanceToLine(point, startLine, endLine);
    double distanceToStart = SphericalUtil::computeDistanceBetween(point, startLine);
    double distanceToEnd   = SphericalUtil::computeDistanceBetween(point, endLine);

    EXPECT_TRUE(distance <= distanceToStart && distance <= distanceToEnd);
}

TEST(PolyUtil, distanceToLine_endpoints) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point at start: distance == 0
    EXPECT_NEAR(PolyUtil::distanceToLine(startLine, startLine, endLine), 0.0, 1e-6);

    // Point at end: distance == 0
    EXPECT_NEAR(PolyUtil::distanceToLine(endLine, startLine, endLine), 0.0, 1e-6);
}

TEST(PolyUtil, distanceToLine_degenerate) {
    LatLng startLine(28.05359, -82.41632);
    LatLng point(28.05342, -82.41594);

    // Degenerate segment (start == end): falls back to computeDistanceBetween
    EXPECT_NEAR(PolyUtil::distanceToLine(point, startLine, startLine),
                SphericalUtil::computeDistanceBetween(point, startLine), 1e-6);
}

TEST(PolyUtil, distanceToLine_beforeStart) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point north of start projects before the segment (u < 0): returns dist to start
    LatLng northOfStart(28.054, -82.41632);
    EXPECT_NEAR(PolyUtil::distanceToLine(northOfStart, startLine, endLine),
                SphericalUtil::computeDistanceBetween(northOfStart, startLine), 1e-6);
}

TEST(PolyUtil, distanceToLine_afterEnd) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point south of end projects past the segment (u > 1): returns dist to end
    LatLng southOfEnd(28.052, -82.41632);
    EXPECT_NEAR(PolyUtil::distanceToLine(southOfEnd, startLine, endLine),
                SphericalUtil::computeDistanceBetween(southOfEnd, endLine), 1e-6);
}
