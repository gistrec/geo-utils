# Getting Started

## Requirements

- C++17 or later
- CMake 3.14 or later

## Integration

### FetchContent (recommended)

```cmake
include(FetchContent)

FetchContent_Declare(
    GeoUtilsCpp
    GIT_REPOSITORY https://github.com/gistrec/geo-utils-cpp.git
    GIT_TAG        v1.0.1
)
FetchContent_MakeAvailable(GeoUtilsCpp)

target_link_libraries(your_target PRIVATE geo::utils)
```

### vcpkg

The library is available in the official [vcpkg registry](https://github.com/microsoft/vcpkg).

Classic mode:

```sh
vcpkg install geo-utils-cpp
```

Manifest mode — add to your `vcpkg.json`:

```json
{
  "dependencies": [
    "geo-utils-cpp"
  ]
}
```

Then consume it from CMake:

```cmake
find_package(GeoUtilsCpp 1.0.1 REQUIRED)
target_link_libraries(your_target PRIVATE geo::utils)
```

### xrepo

The library is available in the official
[xmake-repo](https://github.com/xmake-io/xmake-repo) registry as
`geo-utils-cpp`.

Install via the `xrepo` CLI:

```sh
xrepo install geo-utils-cpp
```

Or declare it as a dependency in your `xmake.lua`:

```lua
add_requires("geo-utils-cpp")

target("your_target")
    set_kind("binary")
    add_files("main.cpp")
    add_packages("geo-utils-cpp")
```

A minimal smoke-test consumer (`tests/consumer/`) is included for both CMake
and xmake builds — see [`tests/consumer/xmake.lua`](../tests/consumer/xmake.lua)
for the xmake variant.

### find_package

Install the library first, then:

```cmake
find_package(GeoUtilsCpp 1.0.1 REQUIRED)
target_link_libraries(your_target PRIVATE geo::utils)
```

### Manual

Copy the `include/` directory into your project and add it to your compiler's include path:

```sh
g++ main.cpp -std=c++17 -I/path/to/geo-utils-cpp/include -o main
```

## Usage

Include the umbrella header or individual modules:

```cpp
// Everything at once
#include <geo/geo.hpp>

// Or individual modules
#include <geo/latlng.hpp>
#include <geo/spherical.hpp>
#include <geo/poly.hpp>
```

### Example: distance and heading between two points

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

### Example: approximate equality with custom tolerance

`LatLng::operator==` is an approximate comparison with tolerance `1e-12` degrees
(≈ 0.1 nm). For comparisons at coarser scale — e.g. metre precision — pass an
explicit tolerance to `approx_equal`:

```cpp
#include <geo/spherical.hpp>

geo::LatLng start{40.0, -74.0};
geo::LatLng end = geo::offset(start, 100'000.0, 90.0);     // 100 km east
auto recovered  = geo::offset_origin(end, 100'000.0, 90.0);

assert(recovered.has_value());
assert(recovered->approx_equal(start, 1e-6));   // ~10 cm tolerance
```

### Example: point-in-polygon

```cpp
#include <iostream>
#include <vector>

#include <geo/poly.hpp>

int main() {
    std::vector<geo::LatLng> polygon = {
        { 40.7650, -73.9900 },
        { 40.7650, -73.9700 },
        { 40.7450, -73.9700 },
        { 40.7450, -73.9900 },
    };

    geo::LatLng timesSquare = { 40.7580, -73.9855 };

    std::cout << std::boolalpha;
    std::cout << geo::contains(timesSquare, polygon) << "\n"; // true
}
```

## Build options

- `GEO_UTILS_CPP_BUILD_TESTS`    — build unit tests (default: ON if top-level, OFF otherwise)
- `GEO_UTILS_CPP_BUILD_EXAMPLES` — build examples   (default: ON if top-level, OFF otherwise)
- `GEO_UTILS_CPP_ENABLE_COVERAGE` — gcov instrumentation, GCC/Clang only (default: OFF)

## Building and testing

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```
