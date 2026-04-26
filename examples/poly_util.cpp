#include <iostream>
#include <vector>

#include "PolyUtil.hpp"

int main() {
    // Bounding box around Midtown Manhattan
    std::vector<LatLng> polygon = {
        { 40.7650, -73.9900 },  // northwest
        { 40.7650, -73.9700 },  // northeast
        { 40.7450, -73.9700 },  // southeast
        { 40.7450, -73.9900 },  // southwest
    };

    LatLng timesSquare = { 40.7580, -73.9855 };  // inside
    LatLng jfk         = { 40.6413, -73.7781 };  // outside (airport)

    std::cout << std::boolalpha;
    std::cout << "Times Square inside polygon: " << PolyUtil::containsLocation(timesSquare, polygon) << "\n";
    std::cout << "JFK inside polygon: "          << PolyUtil::containsLocation(jfk,         polygon) << "\n";

    LatLng segStart = { 40.7650, -73.9900 };
    LatLng segEnd   = { 40.7450, -73.9900 };
    double dist = PolyUtil::distanceToLine(timesSquare, segStart, segEnd);
    std::cout << "Times Square to west boundary: " << dist / 1000.0 << " km\n";

    LatLng onEdge = { 40.7550, -73.9900 };
    std::cout << "Point on polygon edge: " << PolyUtil::isLocationOnEdge(onEdge, polygon) << "\n";

    std::vector<LatLng> route = { { 40.7580, -73.9900 }, { 40.7580, -73.9810 } };
    std::cout << "Times Square on route: " << PolyUtil::isLocationOnPath(timesSquare, route) << "\n";

    return 0;
}
