# API Reference

All public symbols live in the `geo::` namespace. Helpers under `geo::detail::`
are internal and not part of the supported API.

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
#include <geo/latlng.hpp>
```

`geo::LatLng` — a point in geographical coordinates: latitude and longitude.

* Latitude  ranges between `-90` and `90` degrees, inclusive
* Longitude ranges between `-180` and `180` degrees, inclusive. Note: `180` and `-180` are treated as equal.

```cpp
geo::LatLng northPole{90, 0};
geo::LatLng otherPoint = northPole;
```

### Equality

`operator==` performs an **approximate** comparison with tolerance
`LatLng::kDefaultEpsilon` (= `1e-12` degrees, ≈ 0.1 nanometers on Earth).
Longitudes are compared modulo 360°, so `LatLng(0, 180) == LatLng(0, -180)`.

For custom tolerance — e.g. comparing computation results at meter scale —
use `approx_equal`:

```cpp
geo::LatLng a{40.7128, -74.0060};
geo::LatLng b{40.71280001, -74.00599999};

a == b;                    // false (off by ~1e-7°, exceeds default 1e-12)
a.approx_equal(b, 1e-5);   // true (1e-5° ≈ 1 m on equator)
```

---

## Path

A series of connected coordinates in an ordered sequence.

`Path` is a template parameter accepted by `path_length`, `area`, `signed_area`,
`contains`, `on_edge`, and `on_path`. It must be a random-access container of
`geo::LatLng` — specifically, it must support:

- `path.size()` returning a size in elements
- `path[i]` returning a `LatLng` (or something convertible) for `0 ≤ i < size`

This includes `std::vector`, `std::array`, `std::span` (C++20), and
`std::deque`. Forward-only containers like `std::list`, and `std::initializer_list`
(no `operator[]`), are **not** supported — wrap them in a `std::vector` first.

```cpp
std::vector<geo::LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };
std::array<geo::LatLng, 1U> northPole = { {90, 0} };
```

---

## Spherical functions

Spherical geometry utilities for computing angles, distances, and areas.

```cpp
#include <geo/spherical.hpp>
```

### heading

**`geo::heading(const LatLng& from, const LatLng& to)`** — Returns the heading from one LatLng to another. Headings are expressed in degrees clockwise from North within the range `[-180, 180)`.

* `from` — the starting point
* `to` — the destination point

Returns: `double` — the heading in degrees clockwise from north

```cpp
geo::LatLng front{0,  0};
geo::LatLng right{0, 90};

std::cout << geo::heading(right, front); // -90
std::cout << geo::heading(front, right); // +90
```

---

### offset

**`geo::offset(const LatLng& from, double distance, double heading)`** — Returns the LatLng resulting from moving a distance from an origin in the specified heading (degrees clockwise from north).

* `from` — the starting point
* `distance` — the distance to travel, in meters
* `heading` — the heading in degrees clockwise from north

Returns: `LatLng` — the destination point

```cpp
geo::LatLng front{0, 0};

// quarter-circumference of Earth ≈ 10,007.5 km
constexpr double quarter = 10'007'543.4;

auto up    = geo::offset(front, quarter,   0); // {  90,    0}
auto down  = geo::offset(front, quarter, 180); // { -90,    0}
auto left  = geo::offset(front, quarter, -90); // {   0,  -90}
auto right = geo::offset(front, quarter,  90); // {   0,   90}
```

---

### offset_origin

**`geo::offset_origin(const LatLng& to, double distance, double heading)`** — Returns the origin point that, when travelling `distance` meters at `heading`, arrives at `to`. Returns `std::nullopt` when no solution exists.

* `to` — the destination point
* `distance` — the distance travelled, in meters
* `heading` — the heading in degrees clockwise from north

Returns: `std::optional<LatLng>` — the origin, or `std::nullopt` if unreachable

```cpp
geo::LatLng front{0, 0};

auto r0 = geo::offset_origin(front, 0, 0);
assert(r0.has_value() && front == r0.value());
```

---

### interpolate

**`geo::interpolate(const LatLng& from, const LatLng& to, double fraction)`** — Returns the LatLng which lies the given fraction of the way between the origin and the destination (spherical linear interpolation).

* `from` — the starting point
* `to` — the destination point
* `fraction` — a value in `[0, 1]`

Returns: `LatLng`

```cpp
geo::LatLng up{90, 0};
geo::LatLng front{0, 0};

assert(geo::LatLng{1,  0} == geo::interpolate(front, up,  1 / 90.0));
assert(geo::LatLng{89, 0} == geo::interpolate(front, up, 89 / 90.0));
```

---

### angle_between

**`geo::angle_between(const LatLng& from, const LatLng& to)`** — Returns the central angle between two points, in radians.

Returns: `double`

---

### distance_between

**`geo::distance_between(const LatLng& from, const LatLng& to)`** — Returns the distance between two points, in meters.

Returns: `double`

```cpp
geo::LatLng up{90, 0};
geo::LatLng down{-90, 0};

std::cout << geo::distance_between(up, down); // ~2.00151e+07
```

---

### path_length

**`geo::path_length(const Path& path)`** — Returns the length of the given path, in meters.

Returns: `double`

```cpp
std::vector<geo::LatLng> path = { {0, 0}, {90, 0}, {0, 90} };
std::cout << geo::path_length(path); // ~20,015,087 m (pi*R)
```

---

### area

**`geo::area(const Path& path)`** — Returns the area of a closed path on Earth, in square meters.

Returns: `double`

```cpp
// Lune bounded by meridians 0 and 90 — one quarter of the Earth's surface.
std::vector<geo::LatLng> path = { {0, 90}, {-90, 0}, {0, 0}, {90, 0}, {0, 90} };
std::cout << geo::area(path); // ~pi*R^2 (one quarter of the total 4*pi*R^2)
```

---

### signed_area

**`geo::signed_area(const Path& path)`** — Returns the signed area of a closed path on Earth, in square meters.

Sign convention: **counter-clockwise** when viewed from outside the
"inside" face of the polygon yields a **positive** result; clockwise yields a
negative result. "Inside" is the surface that does not contain the South Pole,
so for a small polygon in the northern hemisphere CCW means CCW as seen from
above the North Pole.

Returns: `double`

```cpp
// Triangle in the northern hemisphere, CCW when viewed from above the North Pole
std::vector<geo::LatLng> ccw = { {0, 0}, {0, 10}, {10, 0}, {0, 0} };
std::vector<geo::LatLng> cw  = { {0, 0}, {10, 0}, {0, 10}, {0, 0} };

assert(geo::signed_area(ccw) >  0);
assert(geo::signed_area(cw)  <  0);
assert(geo::signed_area(ccw) == -geo::signed_area(cw));
```

---

## Polygon functions

Utilities for computations involving polygons and polylines.

```cpp
#include <geo/poly.hpp>
```

### contains

**`geo::contains(const LatLng& point, const Path& polygon, bool geodesic = false)`** — Returns whether the given point lies inside the specified polygon. The polygon is always considered closed. The South Pole is always outside.

* `geodesic` — `true` for great circle edges, `false` for rhumb edges

Returns: `bool`

```cpp
std::vector<geo::LatLng> aroundNorthPole = { {89, 0}, {89, 120}, {89, -120} };

std::cout << geo::contains(geo::LatLng{90, 0},  aroundNorthPole); // true
std::cout << geo::contains(geo::LatLng{-90, 0}, aroundNorthPole); // false
```

---

### on_edge

**`geo::on_edge(const LatLng& point, const Path& polygon, bool geodesic = true, double tolerance = geo::kDefaultTolerance)`** — Returns whether the given point lies on or near a polygon edge, within `tolerance` meters.

Returns: `bool`

```cpp
std::vector<geo::LatLng> equator = { {0, 90}, {0, 180} };

std::cout << geo::on_edge(geo::LatLng{0, 90 - 5e-7}, equator); // true
std::cout << geo::on_edge(geo::LatLng{0, 90 - 2e-6}, equator); // false
```

---

### on_path

**`geo::on_path(const LatLng& point, const Path& polyline, bool geodesic = true, double tolerance = geo::kDefaultTolerance)`** — Returns whether the given point lies on or near a polyline, within `tolerance` meters. The closing segment between the first and last points is **not** included.

Returns: `bool`

---

### distance_to_segment

**`geo::distance_to_segment(const LatLng& point, const LatLng& start, const LatLng& end)`** — Returns the distance in meters from `point` to the line segment `[start, end]` on the sphere.

Returns: `double`

```cpp
geo::LatLng start{28.05359, -82.41632};
geo::LatLng end{28.05310, -82.41634};
geo::LatLng point{28.05342, -82.41594};

std::cout << geo::distance_to_segment(point, start, end); // ~37.95
```

---

## Constants

| Symbol | Value | Description |
|---|---|---|
| `geo::kDefaultTolerance` | `0.1` | Default tolerance in meters for `on_edge` / `on_path` |
