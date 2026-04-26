# C++ Geometry Library

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

Header-only C++ library for geographic (lat/lng) geometry (no dependencies).

Provides utilities for distance, bearing, polygon area,
point-in-polygon, and path proximity checks on Earth coordinates.

API inspired by Google Maps geometry utilities.
Uses spherical Earth approximation (like Google Maps).

## Features

* **SphericalUtil** — distance, bearing, area, interpolation
* **PolyUtil** — point-in-polygon, path proximity, distance to segments

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
    CppGeometryLibrary
    GIT_REPOSITORY https://github.com/gistrec/cpp-geometry-library.git
    GIT_TAG        v1.0.0
)
FetchContent_MakeAvailable(CppGeometryLibrary)

target_link_libraries(your_target PRIVATE CppGeometryLibrary::CppGeometryLibrary)
```

### find_package

```cmake
find_package(CppGeometryLibrary REQUIRED)
target_link_libraries(your_target PRIVATE CppGeometryLibrary::CppGeometryLibrary)
```

### Manual

Copy the `include/` directory into your project and add it to your include path.

For more details see [docs/getting-started.md](docs/getting-started.md).

## Usage

```cpp
#include <iostream>
#include <vector>

#include <CppGeometryLibrary/LatLng.hpp>
#include <CppGeometryLibrary/SphericalUtil.hpp>

int main() {
    LatLng newYork = { 40.7128, -74.0060 };
    LatLng london  = { 51.5074,  -0.1278 };

    double distance = SphericalUtil::computeDistanceBetween(newYork, london);
    double heading  = SphericalUtil::computeHeading(newYork, london);

    std::cout << "Distance: " << distance / 1000.0 << " km\n";
    std::cout << "Heading:  " << heading << " deg\n";
}
```

## API Reference

See [docs/api.md](docs/api.md) for the full API reference.

## Support

[Please open an issue on GitHub](https://github.com/gistrec/cpp-geometry-library/issues)

## License

Licensed under the Apache License, Version 2.0. See [LICENSE](LICENSE) for details.
