#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

#include <geo/spherical.hpp>

int main() {
    geo::LatLng newYork = { 40.7128, -74.0060 };
    geo::LatLng london  = { 51.5074,  -0.1278 };
    geo::LatLng paris   = { 48.8566,   2.3522 };

    // distance_between: Great-circle distance in meters.
    double dist = geo::distance_between(newYork, london);
    std::cout << "New York to London: " << dist / 1000.0 << " km\n";
    assert(std::abs(dist - 5'570'000.0) < 5'000.0);  // ~5570 km, ±5 km

    // heading: Initial bearing in degrees clockwise from north.
    double heading = geo::heading(newYork, london);
    std::cout << "Heading New York -> London: " << heading << " deg\n";
    assert(heading > 0 && heading < 90);  // northeast

    // offset: Move a distance from a point along a heading.
    geo::LatLng east = geo::offset(newYork, 500'000, 90.0);
    std::cout << "500 km east of New York: " << east << "\n";
    assert(east.lng > newYork.lng);

    // offset_origin: Inverse of offset — recover the starting point.
    auto origin = geo::offset_origin(east, 500'000, 90.0);
    assert(origin.has_value());
    std::cout << "Recovered origin: " << *origin << "\n";
    assert(origin->approx_equal(newYork, 1e-6));

    // interpolate: Spherical linear interpolation along a great circle.
    geo::LatLng mid = geo::interpolate(newYork, london, 0.5);
    std::cout << "Midpoint NY-London: " << mid << "\n";

    // path_length: Total length of a polyline.
    std::vector<geo::LatLng> route = { newYork, paris, london };
    double length = geo::path_length(route);
    std::cout << "Route NY-Paris-London: " << length / 1000.0 << " km\n";
    assert(length > dist);  // a detour through Paris is longer than direct

    // area: Area enclosed by a closed path.
    double a = geo::area(route);
    std::cout << "NY-Paris-London triangle: " << a / 1.0e6 << " km2\n";

    return 0;
}
