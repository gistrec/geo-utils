# geo-utils

<p align="left">
    <a href="https://github.com/gistrec/geo-utils/actions/workflows/ci.yml">
        <img src="https://github.com/gistrec/geo-utils/actions/workflows/ci.yml/badge.svg" alt="CI">
    </a>
    <a href="https://app.codacy.com/gh/gistrec/geo-utils/dashboard">
      <img src="https://img.shields.io/codacy/grade/bcff544711544d5fb7da95b68abf566d" alt="Code quality">
    </a>
    <a href="https://codecov.io/gh/gistrec/geo-utils">
      <img src="https://codecov.io/gh/gistrec/geo-utils/graph/badge.svg" alt="Coverage">
    </a>
    <a href="https://github.com/gistrec/geo-utils/releases">
        <img src="https://img.shields.io/github/v/release/gistrec/geo-utils" alt="Release">
    </a>
</p>
<p align="left">
    <a href="#">
      <img src="https://img.shields.io/badge/C%2B%2B-17-blue" alt="C++17">
    </a>
    <a href="#">
      <img src="https://img.shields.io/badge/CMake-3.14%2B-064F8C?logo=cmake&logoColor=white" alt="CMake 3.14+">
    </a>
    <a href="#">
      <img src="https://img.shields.io/badge/header--only-yes-brightgreen" alt="Header-only">
    </a>
    <a href="#">
      <img src="https://img.shields.io/badge/platform-Linux%20%C2%B7%20macOS%20%C2%B7%20Windows-brightgreen" alt="Supported platforms">
    </a>
    <a href="https://github.com/gistrec/geo-utils/blob/master/LICENSE">
        <img src="https://img.shields.io/github/license/gistrec/geo-utils?color=brightgreen" alt="License">
    </a>
</p>

Header-only C++17 library for geographic (lat/lng) geometry (no dependencies).

Provides utilities for distance, bearing, polygon area, point-in-polygon, and
path proximity checks on Earth coordinates.

API inspired by Google Maps geometry utilities.
Uses spherical Earth approximation (like Google Maps).

## Features

* **`geo::` spherical functions** — distance, bearing, area, interpolation
* **`geo::` polygon functions** — point-in-polygon, path proximity, distance to segments

## Why use this library?

- Lightweight and header-only (no dependencies)
- Simple API for common GPS/lat-lng calculations
- Suitable for backend, GIS, navigation and tracking systems

## When not to use

- If you need high-precision geodesic calculations on an ellipsoid
- If you need advanced spatial indexing (use S2 / CGAL instead)

## Installation

### FetchContent (recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
    GeoUtils
    GIT_REPOSITORY https://github.com/gistrec/geo-utils.git
    GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(GeoUtils)

target_link_libraries(your_target PRIVATE geo::utils)
```

### find_package

```cmake
find_package(GeoUtils 1.0 REQUIRED)
target_link_libraries(your_target PRIVATE geo::utils)
```

### Manual

Copy the `include/` directory into your project and add it to your include path.

For more details see [docs/getting-started.md](docs/getting-started.md).

## Usage

```cpp
#include <iostream>

#include <geo/spherical.hpp>

int main() {
    geo::LatLng newYork = { 40.7128, -74.0060 };
    geo::LatLng london  = { 51.5074,  -0.1278 };

    double distance = geo::distance_between(newYork, london);
    double heading  = geo::heading(newYork, london);

    std::cout << "Distance: " << distance / 1000.0 << " km\n";
    std::cout << "Heading:  " << heading << " deg\n";
}
```

## API Reference

See [docs/api.md](docs/api.md) for the full API reference.

## Support

[Please open an issue on GitHub](https://github.com/gistrec/geo-utils/issues)

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
