# Changelog

## v1.0.0

Initial stable release. Header-only C++17 library for spherical geographic
geometry, no runtime dependencies.

### API

- All public symbols live in the `geo::` namespace; internal helpers in `geo::detail::`.
- `geo::LatLng` — `struct` with `constexpr` constructor, antimeridian-aware
  `operator==`, tunable `approx_equal(other, eps)`, and `operator<<` for
  debugging.
- Spherical functions: `geo::heading`, `geo::offset`, `geo::offset_origin`,
  `geo::interpolate`, `geo::angle_between`, `geo::distance_between`,
  `geo::path_length`, `geo::area`, `geo::signed_area`.
- Polygon / polyline functions: `geo::contains`, `geo::on_edge`,
  `geo::on_path`, `geo::distance_to_segment`.
- Public constants: `geo::kDefaultTolerance`, `geo::LatLng::kDefaultEpsilon`.
- All non-template functions are `noexcept`.

### Headers

- Umbrella header `<geo/geo.hpp>`; per-module includes
  `<geo/latlng.hpp>`, `<geo/spherical.hpp>`, `<geo/poly.hpp>`.
- `#pragma once` throughout. No `M_PI` macro: π is provided as
  `geo::detail::kPi` constant (portable to MSVC without `_USE_MATH_DEFINES`).

### Build & integration

- CMake 3.14+ required.
- Imported target `geo::utils`.
- `find_package(GeoUtils 1.0 REQUIRED)` after `cmake --install`.
- `FetchContent_Declare(GeoUtils ...)` for in-tree consumption.
- `SameMajorVersion` package compatibility.
- `ENABLE_COVERAGE` option for gcov instrumentation (GCC/Clang).

### Tests & examples

- GoogleTest-based unit tests covering LatLng equality, math helpers,
  spherical functions, and polygon/polyline operations — including edge
  cases at poles, antipodes, and the antimeridian.
- Self-verifying examples (`examples/spherical.cpp`, `examples/poly.cpp`)
  registered as ctest tests.
- Standalone consumer fixture (`tests/consumer/`) for downstream
  `find_package` smoke tests.

### CI

- Multi-OS / multi-compiler matrix: Ubuntu (gcc, clang), macOS (AppleClang),
  Windows (MSVC).
- Separate workflows: build & test (`ci.yml`), install + consumer integration
  on three OSes (`install.yml`), code coverage via Codecov (`coverage.yml`).
- All workflows runnable manually via `workflow_dispatch`.
