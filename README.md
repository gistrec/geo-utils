# Geometry Library

<p align="left">
    <a href="https://github.com/gistrec/cpp-geometry-library/actions/workflows/ci.yml">
        <img src="https://github.com/gistrec/cpp-geometry-library/actions/workflows/ci.yml/badge.svg" alt="CI">
    </a>
    <a href="https://app.codacy.com/gh/gistrec/cpp-geometry-library/dashboard">
      <img src="https://img.shields.io/codacy/grade/bcff544711544d5fb7da95b68abf566d" alt="Code quality">
    </a>
    <a href="https://codecov.io/gh/gistrec/cpp-geometry-library">
      <img src="https://codecov.io/gh/gistrec/cpp-geometry-library/graph/badge.svg" alt="Coverage">
    </a>
    <a href="https://github.com/gistrec/cpp-geometry-library/releases">
        <img src="https://img.shields.io/github/v/release/gistrec/cpp-geometry-library" alt="Release">
    </a>
    <a href="#">
      <img src="https://img.shields.io/badge/platform-windows%20%7C%20linux%20%7C%20osx-brightgreen" alt="Supported platforms">
    </a>
    <a href="https://github.com/gistrec/cpp-geometry-library/blob/master/LICENSE">
        <img src="https://img.shields.io/github/license/gistrec/cpp-geometry-library?color=brightgreen" alt="License">
    </a>
</p>

C++ Geometry Library provides utility functions for the computation of geometric data on the surface of the Earth. Code ported from Google [Maps Android API](https://github.com/googlemaps/android-maps-utils/).

## Features

* [Spherical](https://developers.google.com/maps/documentation/javascript/reference#spherical) contains spherical geometry utilities allowing you to compute angles, distances and areas from latitudes and longitudes.
* [Poly](https://developers.google.com/maps/documentation/javascript/reference#poly) utility functions for computations involving polygons and polylines.

## Installation

### CMake (recommended)

```cmake
find_package(CppGeometryLibrary REQUIRED)
target_link_libraries(your_target PRIVATE CppGeometryLibrary::CppGeometryLibrary)
```

Or embed directly with `add_subdirectory`:

```cmake
add_subdirectory(cpp-geometry-library)
target_link_libraries(your_target PRIVATE CppGeometryLibrary::CppGeometryLibrary)
```

### Manual

Copy the `include/` directory into your project and add it to your include path.

## Usage

You just need to include `SphericalUtil.hpp` or `PolyUtil.hpp`

Here is an example of using this library:

```c++
#include <iostream>
#include <vector>

#include "SphericalUtil.hpp"

int main() {
    LatLng up    = { 90.0,    0.0 };
    LatLng down  = {-90.0,    0.0 };
    LatLng front = {  0.0,    0.0 };
    LatLng right = {  0.0,   90.0 };

    auto angle = SphericalUtil::computeAngleBetween(up, right); // 90
    std::cout << "The angle between up and right is " << rad2deg(angle) << " degrees" << std::endl;

    auto distance = SphericalUtil::computeDistanceBetween(up, down); // 2.00151e+07
    std::cout << "The distance between up and down is " << distance << " meters" << std::endl;

    std::vector<LatLng> points = { front, up, right };

    auto length = SphericalUtil::computeLength(points); // 2.00151e+07
    std::cout << "The length between front, up and right is " << length << " meters" << std::endl;

    auto area = SphericalUtil::computeArea(points); // 6.37582e+13
    std::cout << "The area between front, up and right is " << area << " square meters" << std::endl;

    return 0;
}
```

## Available methods

### PolyUtil class

* [`containsLocation(LatLng point, LatLngList polygon,  bool geodesic)`](#containsLocation)
* [`isLocationOnEdge(LatLng point, LatLngList polygon,  bool geodesic, double tolerance)`](#isLocationOnEdge)
* [`isLocationOnPath(LatLng point, LatLngList polyline, bool geodesic, double tolerance)`](#isLocationOnPath)
* [`distanceToLine(LatLng point, LatLng start, LatLng end)`](#distanceToLine)

### SphericalUtil class

* [`computeHeading(LatLng from, LatLng to)`](#computeHeading)
* [`computeOffset(LatLng from, double distance, double heading)`](#computeOffset)
* [`computeOffsetOrigin(LatLng to, double distance, double heading)`](#computeOffsetOrigin)
* [`interpolate(LatLng from, LatLng to, double fraction)`](#interpolate)
* [`computeDistanceBetween(LatLng from, LatLng to)`](#computeDistanceBetween)
* [`computeLength(LatLngList path)`](#computeLength)
* [`computeArea(LatLngList path)`](#computeArea)
* [`computeSignedArea(LatLngList path)`](#computeSignedArea)

## Classes description

`LatLng` - a point in geographical coordinates: latitude and longitude.

* Latitude  ranges between `-90` and `90` degrees, inclusive
* Longitude ranges between `-180` and `180` degrees, inclusive

Usage example:

```c++
LatLng northPole = {90, 0};

LatLng otherPoint = northPole;
```

---

`LatLngList` - a series of connected coordinates in an ordered sequence. Any iterable containers.

Usage example:

```c++
std::vector<LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };

std::array<LatLng, 1U> northPole = { {90, 0} };
```

## Functions description

### PolyUtil functions

<a name="containsLocation"></a>
**`PolyUtil::containsLocation(const LatLng& point, const LatLngList& polygon, bool geodesic = false)`** - Computes whether the given point lies inside the specified polygon

* `point` - a point in geographical coordinates: latitude and longitude
* `polygon` - a series of connected coordinates in an ordered sequence
* `geodesic` - the polyline is composed of great circle segments if geodesic is true, and of Rhumb segments otherwise

Return value: `bool` - whether the given point lies inside the specified polygon

```c++
// Around the north pole.
std::vector<LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };

std::cout << PolyUtil::containsLocation(LatLng(90, 0), aroundNorthPole);  // true
std::cout << PolyUtil::containsLocation(LatLng(-90, 0), aroundNorthPole); // false
```

---

<a name="isLocationOnEdge"></a>
**`PolyUtil::isLocationOnEdge(const LatLng& point, const LatLngList& polygon, bool geodesic = true, double tolerance = PolyUtil::DEFAULT_TOLERANCE)`** - Computes whether the given point lies on or near to a polyline, or the edge of a polygon, within a specified tolerance. Returns true when the difference between the latitude and longitude of the supplied point, and the closest point on the edge, is less than the tolerance. The tolerance defaults to `0.1` meters.

* `point` - a point in geographical coordinates: latitude and longitude
* `polygon` - a series of connected coordinates in an ordered sequence
* `geodesic` - the polyline is composed of great circle segments if geodesic is true, and of Rhumb segments otherwise
* `tolerance` - tolerance value in meters

Return value: `bool` - whether the given point lies on or near the edge of a polygon

```c++
// On equator.
std::vector<LatLng> equator = { {0, 90}, {0, 180} };

double small = 5e-7; // Half the default tolerance.
double big   = 2e-6; // Double the default tolerance.

std::cout << PolyUtil::isLocationOnEdge(LatLng(0, 90 - small), equator); // true
std::cout << PolyUtil::isLocationOnEdge(LatLng(0, 90 - big),   equator); // false
```

---

<a name="isLocationOnPath"></a>
**`PolyUtil::isLocationOnPath(const LatLng& point, const LatLngList& polyline, bool geodesic = true, double tolerance = PolyUtil::DEFAULT_TOLERANCE)`** - Computes whether the given point lies on or near a polyline, within a specified tolerance in meters. The polyline is composed of great circle segments if geodesic is true, and of Rhumb segments otherwise. The polyline is not closed -- the closing segment between the first point and the last point is not included.

* `point` - a point in geographical coordinates: latitude and longitude
* `polygon` - a series of connected coordinates in an ordered sequence
* `geodesic` - the polyline is composed of great circle segments if geodesic is true, and of Rhumb segments otherwise
* `tolerance` - tolerance value in meters

Return value: `bool` - whether the point lies on or near a polyline

```c++
// On equator.
std::vector<LatLng> equator = { {0, 90}, {0, 180} };

double small = 5e-7; // Half the default tolerance.
double big   = 2e-6; // Double the default tolerance.

std::cout << PolyUtil::isLocationOnPath(LatLng(0, 90 - small), equator); // true
std::cout << PolyUtil::isLocationOnPath(LatLng(0, 90 - big),   equator); // false
```

---

<a name="distanceToLine"></a>
**`PolyUtil::distanceToLine(const LatLng& p, const LatLng& start, const LatLng& end)`** - Computes the distance on the sphere between the point p and the line segment start to end.

* `point` - the point to be measured
* `start` - the beginning of the line segment
* `end` - the end of the line segment

Return value: `double` - the distance in meters (assuming spherical earth)

```c++
LatLng startLine(28.05359, -82.41632);
LatLng endLine(28.05310, -82.41634);
LatLng point(28.05342, -82.41594);

std::cout << PolyUtil::distanceToLine(point, startLine, endLine); // 37.947946
```

### SphericalUtil functions

<a name="computeHeading"></a>
**`SphericalUtil::computeHeading(const LatLng& from, const LatLng& to)`** - Returns the heading from one LatLng to another LatLng. Headings are expressed in degrees clockwise from North within the range [-180,180).

* `from` - a point in geographical coordinates: latitude and longitude
* `to` - a point in geographical coordinates: latitude and longitude

Return value: `double` - the heading in degrees clockwise from north

```c++
LatLng front(0,  0);
LatLng right(0, 90);

std::cout << SphericalUtil::computeHeading(right, front); // -90
std::cout << SphericalUtil::computeHeading(front, right); // +90
```

---

<a name="computeOffset"></a>
**`SphericalUtil::computeOffset(const LatLng& from, double distance, double heading)`** - Returns the LatLng resulting from moving a distance from an origin in the specified heading (expressed in degrees clockwise from north).

* `from` - the LatLng from which to start.
* `distance` - the distance to travel.
* `heading` - the heading in degrees clockwise from north.

Return value: `LatLng` - resulting from moving a distance from an origin in the specified heading (expressed in degrees clockwise from north)

```c++
LatLng front(0, 0);

auto up    = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,   0); // LatLng( 90,    0)
auto down  = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, 180); // LatLng(-90,    0)
auto left  = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, -90); // LatLng(  0,  -90)
auto right = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,  90); // LatLng(  0,   90)
auto back  = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS,      90); // LatLng(  0, -180)
```

---

<a name="computeOffsetOrigin"></a>
**`SphericalUtil::computeOffsetOrigin(const LatLng& to, double distance, double heading)`** - Returns the location of origin when provided with a LatLng destination, meters travelled and original heading. Headings are expressed in degrees clockwise from North.

* `from` - the destination LatLng
* `distance` - the distance travelled, in meters.
* `heading` - the heading in degrees clockwise from north

Return value: `std::optional<LatLng>` - the location of origin, or `std::nullopt` when no solution exists (destination unreachable with the given distance and heading)

```c++
LatLng front(0, 0);

auto r0 = SphericalUtil::computeOffsetOrigin(front, 0, 0);
assert(r0.has_value() && front == r0.value());

auto r1 = SphericalUtil::computeOffsetOrigin(LatLng(  0,  45), M_PI * MathUtil::EARTH_RADIUS / 4,  90);
auto r2 = SphericalUtil::computeOffsetOrigin(LatLng(  0, -45), M_PI * MathUtil::EARTH_RADIUS / 4, -90);
auto r3 = SphericalUtil::computeOffsetOrigin(LatLng( 45,   0), M_PI * MathUtil::EARTH_RADIUS / 4,   0);
auto r4 = SphericalUtil::computeOffsetOrigin(LatLng(-45,   0), M_PI * MathUtil::EARTH_RADIUS / 4, 180);
assert(r1.has_value() && front == r1.value());
assert(r2.has_value() && front == r2.value());
assert(r3.has_value() && front == r3.value());
assert(r4.has_value() && front == r4.value());

// Returns nullopt when destination is unreachable
assert(!SphericalUtil::computeOffsetOrigin(LatLng(80, 0), M_PI * MathUtil::EARTH_RADIUS / 4, 180).has_value());
```

---

<a name="interpolate"></a>
**`SphericalUtil::interpolate(const LatLng& from, const LatLng& to, double fraction)`** - Returns the LatLng which lies the given fraction of the way between the origin LatLng and the destination LatLng.

* `from` - the LatLng from which to start.
* `to` - the LatLng toward which to travel.
* `fraction` - a fraction of the distance to travel.

Return value: `LatLng` - point which lies the given fraction of the way between the origin LatLng and the destination LatLng

```c++
LatLng up(90, 0);
LatLng front(0, 0);

assert(LatLng(1,  0) == SphericalUtil::interpolate(front, up,  1 / 90.0));
assert(LatLng(1,  0) == SphericalUtil::interpolate(up, front, 89 / 90.0));
assert(LatLng(89, 0) == SphericalUtil::interpolate(front, up, 89 / 90.0));
assert(LatLng(89, 0) == SphericalUtil::interpolate(up, front,  1 / 90.0));

```

---

<a name="computeDistanceBetween"></a>
**`SphericalUtil::computeDistanceBetween(const LatLng& from, const LatLng& to)`** - Returns the distance, in meters, between two LatLngs.

* `from` - the first point
* `to` - the second point

Return value: `double` - the distance, in meters, between two LatLngs

```c++
LatLng up(90, 0);
LatLng down(-90, 0);

std::cout << SphericalUtil::computeDistanceBetween(up, down); // MathUtil::EARTH_RADIUS
```

---

<a name="computeLength"></a>
**`SphericalUtil::computeLength(const LatLngList& path)`** - Returns the length of the given path, in meters, on Earth

* `path` - a series of connected coordinates in an ordered sequence. Any iterable containers.

Return valuse: `double` - the length of the given path, in meters, on Earth

```c++
// List with three points
std::vector<LatLng> latLngs2 = { {0, 0}, {90, 0}, {0, 90} };

std::cout << SphericalUtil::computeLength(latLngs2); // M_PI * MathUtil::EARTH_RADIUS
```

---

<a name="computeArea"></a>
**`SphericalUtil::computeArea(const LatLngList& path)`** - Returns the area of a closed path on Earth.

* `path` - a closed path. Any iterable containers.

Return value: `double` - the area of a closed path on Earth

```c++
LatLng up    = { 90.0,  0.0 };
LatLng down  = {-90.0,  0.0 };
LatLng front = {  0.0,  0.0 };
LatLng right = {  0.0, 90.0 };

std::vector<LatLng> path = { right, down, front, up, right };

std::cout << SphericalUtil::computeArea(path); // M_PI * MathUtil::EARTH_RADIUS * MathUtil::EARTH_RADIUS
```

---

<a name="computeSignedArea"></a>
**`SphericalUtil::computeSignedArea(const LatLngList& path)`** - Returns the signed area of a closed path on Earth. The sign of the area may be used to determine the orientation of the path. "inside" is the surface that does not contain the South Pole.

* `path` - a closed path. Any iterable containers.

Return value: `double` - the loop's area in square meters

```c++
LatLng up    = { 90.0,    0.0 };
LatLng down  = {-90.0,    0.0 };
LatLng front = {  0.0,    0.0 };
LatLng right = {  0.0,   90.0 };

std::vector<LatLng> path         = { right,   up, front, down, right };
std::vector<LatLng> pathReversed = { right, down, front,   up, right };

assert(SphericalUtil::computeSignedArea(path) == -SphericalUtil::computeSignedArea(pathReversed));
```

---

## Support

[Please open an issue on GitHub](https://github.com/gistrec/cpp-geometry-library/issues)

## License

Geometry Library Google Maps API V3 is released under the MIT License.
See the bundled [LICENSE](https://github.com/alexpechkarev/geometry-library/blob/master/LICENSE) file for details.
