# API Reference

## Conventions

- Coordinates are in degrees (latitude, longitude)
- Distances are in meters
- Angles are in degrees unless otherwise specified
- Earth model: spherical (mean radius = 6371009 m)

## Numerical notes

- Results are approximate due to floating point arithmetic
- Precision decreases near the poles and for antipodal points

## LatLng

Represents a geographic coordinate (latitude, longitude) in degrees.

```cpp
#include <CppGeometryLibrary/LatLng.hpp>
```

`LatLng` - a point in geographical coordinates: latitude and longitude.

* Latitude  ranges between `-90` and `90` degrees, inclusive
* Longitude ranges between `-180` and `180` degrees, inclusive. Note: `180` and `-180` are treated as equal.

```cpp
LatLng northPole{90, 0};
LatLng otherPoint = northPole;
```

---

## LatLngList

A series of connected coordinates in an ordered sequence. Any iterable container of `LatLng` (e.g. std::vector, std::array).

```cpp
std::vector<LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };
std::array<LatLng, 1U> northPole = { {90, 0} };
```

---

## SphericalUtil

Spherical geometry utilities for computing angles, distances, and areas.

```cpp
#include <CppGeometryLibrary/SphericalUtil.hpp>
```

### computeHeading

**`SphericalUtil::computeHeading(const LatLng& from, const LatLng& to)`** — Returns the heading from one LatLng to another LatLng. Headings are expressed in degrees clockwise from North within the range `[-180, 180)`.

* `from` — the starting point
* `to` — the destination point

Returns: `double` — the heading in degrees clockwise from north

```cpp
LatLng front{0,  0};
LatLng right{0, 90};

std::cout << SphericalUtil::computeHeading(right, front); // -90
std::cout << SphericalUtil::computeHeading(front, right); // +90
```

---

### computeOffset

**`SphericalUtil::computeOffset(const LatLng& from, double distance, double heading)`** — Returns the LatLng resulting from moving a distance from an origin in the specified heading (expressed in degrees clockwise from north).

* `from` — the starting point
* `distance` — the distance to travel, in meters
* `heading` — the heading in degrees clockwise from north

Returns: `LatLng` — the destination point

```cpp
LatLng front{0, 0};

auto up    = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,   0); // LatLng{ 90,    0}
auto down  = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, 180); // LatLng{-90,    0}
auto left  = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2, -90); // LatLng{  0,  -90}
auto right = SphericalUtil::computeOffset(front, M_PI * MathUtil::EARTH_RADIUS / 2,  90); // LatLng{  0,   90}
```

---

### computeOffsetOrigin

**`SphericalUtil::computeOffsetOrigin(const LatLng& to, double distance, double heading)`** — Returns the origin point that, when travelling `distance` meters at `heading`, arrives at `to`. Returns `std::nullopt` when no solution exists.

* `to` — the destination point
* `distance` — the distance travelled, in meters
* `heading` — the heading in degrees clockwise from north

Returns: `std::optional<LatLng>` — the origin, or `std::nullopt` if unreachable

```cpp
LatLng front{0, 0};

auto r0 = SphericalUtil::computeOffsetOrigin(front, 0, 0);
assert(r0.has_value() && front == r0.value());

// Returns nullopt when destination is unreachable
assert(!SphericalUtil::computeOffsetOrigin(LatLng{80, 0}, M_PI * MathUtil::EARTH_RADIUS / 4, 180).has_value());
```

---

### interpolate

**`SphericalUtil::interpolate(const LatLng& from, const LatLng& to, double fraction)`** — Returns the LatLng which lies the given fraction of the way between the origin LatLng and the destination LatLng (spherical linear interpolation).

* `from` — the starting point
* `to` — the destination point
* `fraction` — a value in `[0, 1]`

Returns: `LatLng` — the interpolated point

```cpp
LatLng up{90, 0};
LatLng front{0, 0};

assert(LatLng{1,  0} == SphericalUtil::interpolate(front, up,  1 / 90.0));
assert(LatLng{89, 0} == SphericalUtil::interpolate(front, up, 89 / 90.0));
```

---

### computeAngleBetween

**`SphericalUtil::computeAngleBetween(const LatLng& from, const LatLng& to)`** — Returns the central angle between two points, in radians.

* `from` — the first point
* `to` — the second point

Returns: `double` — the angle in radians

---

### computeDistanceBetween

**`SphericalUtil::computeDistanceBetween(const LatLng& from, const LatLng& to)`** — Returns the distance between two points, in meters.

* `from` — the first point
* `to` — the second point

Returns: `double` — the distance in meters

```cpp
LatLng up{90, 0};
LatLng down{-90, 0};

std::cout << SphericalUtil::computeDistanceBetween(up, down); // ~2.00151e+07
```

---

### computeLength

**`SphericalUtil::computeLength(const LatLngList& path)`** — Returns the length of the given path, in meters.

* `path` — a sequence of points

Returns: `double` — the total path length in meters

```cpp
std::vector<LatLng> path = { {0, 0}, {90, 0}, {0, 90} };
std::cout << SphericalUtil::computeLength(path); // ~M_PI * MathUtil::EARTH_RADIUS
```

---

### computeArea

**`SphericalUtil::computeArea(const LatLngList& path)`** — Returns the area of a closed path on Earth.

* `path` — a closed sequence of points

Returns: `double` — the area in square meters

```cpp
std::vector<LatLng> path = { {0, 90}, {-90, 0}, {0, 0}, {90, 0}, {0, 90} };
std::cout << SphericalUtil::computeArea(path); // ~M_PI * EARTH_RADIUS²
```

---

### computeSignedArea

**`SphericalUtil::computeSignedArea(const LatLngList& path)`** — Returns the signed area of a closed path. The sign indicates orientation: positive for counter-clockwise, negative for clockwise. "Inside" is the surface that does not contain the South Pole.

This follows the convention used in Google Maps geometry utilities.

* `path` — a closed sequence of points

Returns: `double` — the signed area in square meters

```cpp
std::vector<LatLng> path         = { {0, 90}, { 90, 0}, {0, 0}, {-90, 0}, {0, 90} };
std::vector<LatLng> pathReversed = { {0, 90}, {-90, 0}, {0, 0}, { 90, 0}, {0, 90} };

assert(SphericalUtil::computeSignedArea(path) == -SphericalUtil::computeSignedArea(pathReversed));
```

---

## PolyUtil

Utilities for computations involving polygons and polylines.

```cpp
#include <CppGeometryLibrary/PolyUtil.hpp>
```

### containsLocation

**`PolyUtil::containsLocation(const LatLng& point, const LatLngList& polygon, bool geodesic = false)`** — Returns whether the given point lies inside the specified polygon. The polygon is always considered closed. The South Pole is always outside.

* `point` — the point to test
* `polygon` — the polygon vertices
* `geodesic` — `true` for great circle edges, `false` for rhumb edges

Returns: `bool`

```cpp
std::vector<LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };

std::cout << PolyUtil::containsLocation(LatLng{90, 0},  aroundNorthPole); // true
std::cout << PolyUtil::containsLocation(LatLng{-90, 0}, aroundNorthPole); // false
```

---

### isLocationOnEdge

**`PolyUtil::isLocationOnEdge(const LatLng& point, const LatLngList& polygon, bool geodesic = true, double tolerance = 0.1)`** — Returns whether the given point lies on or near a polygon edge, within `tolerance` meters.

* `point` — the point to test
* `polygon` — the polygon vertices
* `geodesic` — `true` for great circle edges, `false` for rhumb edges
* `tolerance` — tolerance in meters (default `0.1`)

Returns: `bool`

```cpp
std::vector<LatLng> equator = { {0, 90}, {0, 180} };

double small = 5e-7; // within tolerance
double big   = 2e-6; // outside tolerance

std::cout << PolyUtil::isLocationOnEdge(LatLng{0, 90 - small}, equator); // true
std::cout << PolyUtil::isLocationOnEdge(LatLng{0, 90 - big},   equator); // false
```

---

### isLocationOnPath

**`PolyUtil::isLocationOnPath(const LatLng& point, const LatLngList& polyline, bool geodesic = true, double tolerance = 0.1)`** — Returns whether the given point lies on or near a polyline, within `tolerance` meters. The closing segment between the first and last points is not included.

* `point` — the point to test
* `polyline` — the polyline vertices
* `geodesic` — `true` for great circle segments, `false` for rhumb segments
* `tolerance` — tolerance in meters (default `0.1`)

Returns: `bool`

```cpp
std::vector<LatLng> equator = { {0, 90}, {0, 180} };

std::cout << PolyUtil::isLocationOnPath(LatLng{0, 90}, equator); // true
```

---

### distanceToLine

**`PolyUtil::distanceToLine(const LatLng& point, const LatLng& start, const LatLng& end)`** — Returns the distance in meters from `point` to the line segment `[start, end]` on the sphere.

* `point` — the point to measure from
* `start` — the start of the segment
* `end` — the end of the segment

Returns: `double` — the distance in meters

```cpp
LatLng start{28.05359, -82.41632};
LatLng end{28.05310, -82.41634};
LatLng point{28.05342, -82.41594};

std::cout << PolyUtil::distanceToLine(point, start, end); // ~37.95
```

---

## MathUtil

Low-level math helpers used internally. Available for direct use if needed.

```cpp
#include <CppGeometryLibrary/MathUtil.hpp>
```

| Symbol | Type | Description |
|---|---|---|
| `MathUtil::EARTH_RADIUS` | `double` | Mean Earth radius: `6371009.0` m (IUGG) |
| `MathUtil::clamp(x, low, high)` | `double` | Clamps `x` to `[low, high]` |
| `MathUtil::wrap(n, min, max)` | `double` | Wraps `n` into `[min, max)` |
| `deg2rad(degrees)` | `double` | Converts degrees to radians |
| `rad2deg(radians)` | `double` | Converts radians to degrees |
