cmake_minimum_required(VERSION 3.14)
project(geo_utils_cpp_consumer LANGUAGES CXX)

# Smoke test for an installed geo-utils-cpp package: locate it via find_package,
# link against geo::utils, and run a minimal program. Intended to be configured
# against an install prefix from the install workflow.

# No version pinned: this is a smoke test for findability/linkage, not a
# version-compatibility check. Version assertions live in install.yml's
# pkg-config step (which derives the expected version from CMakeLists.txt).
find_package(GeoUtilsCpp REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE geo::utils)

enable_testing()
add_test(NAME smoke COMMAND app)
