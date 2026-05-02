#include <gtest/gtest.h>
#include <vector>

#include <geo/poly.hpp>
#include <geo/spherical.hpp>

using geo::LatLng;
using geo::distance_to_segment;
using geo::distance_between;

TEST(Poly, distance_to_segment) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);
    LatLng point(28.05342, -82.41594);

    double distance = distance_to_segment(point, startLine, endLine);

    EXPECT_NEAR(37.947946, distance, 1e-6);
}

TEST(Poly, distance_to_segment_LessThanDistanceToExtrems) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);
    LatLng point(28.05342, -82.41594);

    double distance        = distance_to_segment(point, startLine, endLine);
    double distanceToStart = distance_between(point, startLine);
    double distanceToEnd   = distance_between(point, endLine);

    EXPECT_TRUE(distance <= distanceToStart && distance <= distanceToEnd);
}

TEST(Poly, distance_to_segment_endpoints) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point at start: distance == 0
    EXPECT_NEAR(distance_to_segment(startLine, startLine, endLine), 0.0, 1e-6);

    // Point at end: distance == 0
    EXPECT_NEAR(distance_to_segment(endLine, startLine, endLine), 0.0, 1e-6);
}

TEST(Poly, distance_to_segment_degenerate) {
    LatLng startLine(28.05359, -82.41632);
    LatLng point(28.05342, -82.41594);

    // Degenerate segment (start == end): falls back to distance_between
    EXPECT_NEAR(distance_to_segment(point, startLine, startLine),
                distance_between(point, startLine), 1e-6);
}

TEST(Poly, distance_to_segment_beforeStart) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point north of start projects before the segment (u < 0): returns dist to start
    LatLng northOfStart(28.054, -82.41632);
    EXPECT_NEAR(distance_to_segment(northOfStart, startLine, endLine),
                distance_between(northOfStart, startLine), 1e-6);
}

TEST(Poly, distance_to_segment_afterEnd) {
    LatLng startLine(28.05359, -82.41632);
    LatLng endLine(28.05310, -82.41634);

    // Point south of end projects past the segment (u > 1): returns dist to end
    LatLng southOfEnd(28.052, -82.41632);
    EXPECT_NEAR(distance_to_segment(southOfEnd, startLine, endLine),
                distance_between(southOfEnd, endLine), 1e-6);
}
