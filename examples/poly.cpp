#include <cassert>
#include <iostream>
#include <vector>

#include <geo/poly.hpp>

int main() {
    // Bounding box around Midtown Manhattan.
    std::vector<geo::LatLng> polygon = {
        { 40.7650, -73.9900 },  // northwest
        { 40.7650, -73.9700 },  // northeast
        { 40.7450, -73.9700 },  // southeast
        { 40.7450, -73.9900 },  // southwest
    };

    geo::LatLng timesSquare = { 40.7580, -73.9855 };  // inside
    geo::LatLng jfk         = { 40.6413, -73.7781 };  // outside (airport)

    std::cout << std::boolalpha;

    // contains: Point-in-polygon test.
    bool tsInside = geo::contains(timesSquare, polygon);
    bool jfkInside = geo::contains(jfk, polygon);
    std::cout << "Times Square inside polygon: " << tsInside << "\n";
    std::cout << "JFK inside polygon: "          << jfkInside << "\n";
    assert(tsInside);
    assert(!jfkInside);

    // distance_to_segment: Distance from a point to a finite segment.
    geo::LatLng segStart = { 40.7650, -73.9900 };
    geo::LatLng segEnd   = { 40.7450, -73.9900 };
    double dist = geo::distance_to_segment(timesSquare, segStart, segEnd);
    std::cout << "Times Square to west boundary: " << dist << " m\n";
    assert(dist > 0 && dist < 1000);

    // on_edge: Test whether a point lies on (or near) a polygon's edge.
    geo::LatLng onBoundary = { 40.7550, -73.9900 };
    bool isOnEdge = geo::on_edge(onBoundary, polygon);
    std::cout << "Point on polygon edge: " << isOnEdge << "\n";
    assert(isOnEdge);

    // on_path: Test whether a point lies on (or near) a polyline.
    std::vector<geo::LatLng> route = { { 40.7580, -73.9900 }, { 40.7580, -73.9810 } };
    bool isOnRoute = geo::on_path(timesSquare, route);
    std::cout << "Times Square on route: " << isOnRoute << "\n";
    assert(isOnRoute);

    return 0;
}
