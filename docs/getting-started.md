# Getting Started

## Requirements

- C++17 or later
- CMake 3.14 or later

## Integration

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

Install the library first, then:

```cmake
find_package(CppGeometryLibrary REQUIRED)
target_link_libraries(your_target PRIVATE CppGeometryLibrary::CppGeometryLibrary)
```

### Manual

Copy the `include/` directory into your project and add it to your compiler's include path:

```sh
g++ main.cpp -std=c++17 -I/path/to/cpp-geometry-library/include -o main
```

## Usage

Include the full library or individual modules:

```cpp
// Everything at once
#include <CppGeometryLibrary.hpp>

// Or individual modules
#include <CppGeometryLibrary/SphericalUtil.hpp>
#include <CppGeometryLibrary/PolyUtil.hpp>
```

### Example: distance and heading between two points

```cpp
#include <iostream>

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

### Example: point-in-polygon

```cpp
#include <iostream>
#include <vector>

#include <CppGeometryLibrary/LatLng.hpp>
#include <CppGeometryLibrary/PolyUtil.hpp>

int main() {
    std::vector<LatLng> polygon = {
        { 40.7650, -73.9900 },
        { 40.7650, -73.9700 },
        { 40.7450, -73.9700 },
        { 40.7450, -73.9900 },
    };

    LatLng timesSquare = { 40.7580, -73.9855 };

    std::cout << std::boolalpha;
    std::cout << PolyUtil::containsLocation(timesSquare, polygon) << "\n"; // true
}
```

## Building and testing

```sh
cmake -S . -B build -DBUILD_TESTING=ON
cmake --build build
ctest --test-dir build --output-on-failure
```
