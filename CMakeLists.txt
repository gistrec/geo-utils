cmake_minimum_required(VERSION 3.14)
project(GeoUtils VERSION 1.0.0 LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(_geo_utils_is_top_level ON)
else()
    set(_geo_utils_is_top_level OFF)
endif()

option(GEO_UTILS_BUILD_TESTS    "Build geo-utils tests"    ${_geo_utils_is_top_level})
option(GEO_UTILS_BUILD_EXAMPLES "Build geo-utils examples" ${_geo_utils_is_top_level})
option(ENABLE_COVERAGE          "Enable gcov coverage instrumentation (GCC/Clang only). Requires GEO_UTILS_BUILD_TESTS=ON." OFF)

# Header-only interface target
add_library(geo_utils INTERFACE)
add_library(geo::utils ALIAS geo_utils)

# So the exported imported target is geo::utils (matches the build-tree alias)
set_target_properties(geo_utils PROPERTIES EXPORT_NAME utils)

target_include_directories(geo_utils INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(geo_utils INTERFACE cxx_std_17)

# Tests
if(GEO_UTILS_BUILD_TESTS)
    include(CTest)

    find_package(GTest QUIET)
    if(NOT GTest_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/v1.14.0.tar.gz
            DOWNLOAD_EXTRACT_TIMESTAMP ON
        )
        # Prevent overriding the parent project's compiler/linker settings on Windows
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        # Don't leak GTest into our install prefix
        set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
        FetchContent_MakeAvailable(googletest)
        # FetchContent creates 'gtest'/'gtest_main'; alias to match find_package names
        add_library(GTest::GTest ALIAS gtest)
        add_library(GTest::Main  ALIAS gtest_main)
    endif()

    add_executable(geo_utils_tests tests/tests.cpp)
    target_link_libraries(geo_utils_tests
        PRIVATE geo::utils GTest::GTest GTest::Main
    )
    add_test(NAME geo_utils_tests COMMAND geo_utils_tests)

    if(ENABLE_COVERAGE)
        target_compile_options(geo_utils_tests PRIVATE --coverage)
        target_link_options(geo_utils_tests PRIVATE --coverage)
    endif()
endif()

# Examples
if(GEO_UTILS_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS geo_utils
    EXPORT GeoUtilsTargets
)
install(EXPORT GeoUtilsTargets
    FILE GeoUtilsTargets.cmake
    NAMESPACE geo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeoUtils
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/GeoUtilsConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/GeoUtilsConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeoUtils
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/GeoUtilsConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GeoUtilsConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GeoUtilsConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/GeoUtils
)
