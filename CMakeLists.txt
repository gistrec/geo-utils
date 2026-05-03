cmake_minimum_required(VERSION 3.14)
project(GeoUtils VERSION 1.0.0 LANGUAGES CXX)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(_geo_utils_is_top_level ON)
else()
    set(_geo_utils_is_top_level OFF)
endif()

include(CMakeDependentOption)
option(GEO_UTILS_BUILD_TESTS    "Build geo-utils tests"    ${_geo_utils_is_top_level})
option(GEO_UTILS_BUILD_EXAMPLES "Build geo-utils examples" ${_geo_utils_is_top_level})
cmake_dependent_option(GEO_UTILS_ENABLE_COVERAGE
    "Enable gcov coverage instrumentation (GCC/Clang only)" OFF
    "GEO_UTILS_BUILD_TESTS" OFF)

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
    include(GoogleTest)

    if(TARGET gtest)
        # Someone upstream (the consuming project, or an earlier add_subdirectory)
        # already set up googletest. Reuse those targets — don't try to find or
        # fetch our own. Add the legacy GTest:: aliases only if they're missing.
        if(NOT TARGET GTest::GTest)
            add_library(GTest::GTest ALIAS gtest)
        endif()
        if(NOT TARGET GTest::Main)
            add_library(GTest::Main ALIAS gtest_main)
        endif()
    else()
        find_package(GTest QUIET)
        if(NOT GTest_FOUND)
            include(FetchContent)
            FetchContent_Declare(
                googletest
                URL https://github.com/google/googletest/archive/v1.14.0.tar.gz
                DOWNLOAD_EXTRACT_TIMESTAMP ON
            )
            # Prevent overriding the parent project's compiler/linker settings on Windows.
            # Local (non-cache) variables — relies on CMP0077 (NEW since CMake 3.13,
            # default at our cmake_minimum_required of 3.14) so the option() calls
            # inside googletest don't overwrite these.
            set(gtest_force_shared_crt ON)
            # Don't leak GTest into our install prefix.
            set(INSTALL_GTEST OFF)
            FetchContent_MakeAvailable(googletest)
            # FetchContent creates 'gtest'/'gtest_main'; alias to match find_package names
            add_library(GTest::GTest ALIAS gtest)
            add_library(GTest::Main  ALIAS gtest_main)
        endif()
    endif()

    add_executable(geo_utils_tests tests/tests.cpp)
    target_link_libraries(geo_utils_tests
        PRIVATE geo::utils GTest::GTest GTest::Main
    )
    gtest_discover_tests(geo_utils_tests)

    if(GEO_UTILS_ENABLE_COVERAGE)
        # `--coverage` is split: compile inserts gcov hooks, link pulls in the
        # gcov runtime. The compiler-flag check must include both stages — see
        # CMAKE_REQUIRED_LINK_OPTIONS (CMake 3.14+). Otherwise the test object
        # compiles fine but link fails on missing gcov symbols, giving a false
        # negative on toolchains that actually support the flag (e.g. AppleClang).
        include(CheckCXXCompilerFlag)
        set(CMAKE_REQUIRED_LINK_OPTIONS --coverage)
        check_cxx_compiler_flag(--coverage GEO_UTILS_HAS_COVERAGE_FLAG)
        unset(CMAKE_REQUIRED_LINK_OPTIONS)

        if(NOT GEO_UTILS_HAS_COVERAGE_FLAG)
            message(FATAL_ERROR
                "GEO_UTILS_ENABLE_COVERAGE=ON requested, but the C++ compiler "
                "(${CMAKE_CXX_COMPILER_ID}) does not support --coverage. "
                "Coverage instrumentation requires GCC or Clang.")
        endif()
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

if(_geo_utils_is_top_level)                                                                                                                                                                                                                                                                                                                                               
    set(CPACK_PACKAGE_NAME    "geo-utils")
    set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")                                                                                                                                                                                                                                                                                                                       
    set(CPACK_GENERATOR       "ZIP")                                                                                                                                                                                                                                                                                                                                      
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")                                                                                                                                                                                                                                                                                                
    include(CPack)                                                                                                                                                                                                                                                                                                                                                        
endif()
