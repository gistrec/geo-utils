// Smoke test for an installed geo-utils-cpp package.
// Verifies that <geo/geo.hpp> is reachable, geo::utils target links, and
// the basic public API produces a sensible result.

#include <cassert>
#include <geo/geo.hpp>

int main() {
    geo::LatLng nyc{40.7128, -74.0060};
    geo::LatLng london{51.5074, -0.1278};

    double d = geo::distance_between(nyc, london);
    assert(d > 5'000'000.0 && d < 6'000'000.0);

    return 0;
}
