cmake_minimum_required(VERSION 3.14)
project(geo_utils_cpp_consumer LANGUAGES CXX)

# Smoke test for an installed geo-utils-cpp package: locate it via find_package,
# link against geo::utils, and run a minimal program. Intended to be configured
# against an install prefix from the install workflow.

find_package(GeoUtilsCpp 1.0 REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app PRIVATE geo::utils)

enable_testing()
add_test(NAME smoke COMMAND app)
