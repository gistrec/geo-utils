#include <iostream>
#include <vector>

#include <CppGeometryLibrary/SphericalUtil.hpp>

int main() {
    LatLng newYork = { 40.7128, -74.0060 };
    LatLng london  = { 51.5074,  -0.1278 };
    LatLng paris   = { 48.8566,   2.3522 };

    double dist = SphericalUtil::computeDistanceBetween(newYork, london);
    std::cout << "New York to London: " << dist / 1000.0 << " km\n";

    double heading = SphericalUtil::computeHeading(newYork, london);
    std::cout << "Heading New York -> London: " << heading << " deg\n";

    LatLng east = SphericalUtil::computeOffset(newYork, 500'000, 90.0);
    std::cout << "500 km east of New York: " << east.lat << ", " << east.lng << "\n";

    LatLng mid = SphericalUtil::interpolate(newYork, london, 0.5);
    std::cout << "Midpoint New York-London: " << mid.lat << ", " << mid.lng << "\n";

    std::vector<LatLng> route = { newYork, paris, london };
    double length = SphericalUtil::computeLength(route);
    std::cout << "Route New York-Paris-London: " << length / 1000.0 << " km\n";

    double area = SphericalUtil::computeArea(route);
    std::cout << "Triangle area: " << area / 1.0e6 << " km2\n";

    return 0;
}
