cmake_minimum_required(VERSION 3.14)
project(CppGeometryLibrary VERSION 1.0.0 LANGUAGES CXX)

# Header-only interface target
add_library(CppGeometryLibrary INTERFACE)
add_library(CppGeometryLibrary::CppGeometryLibrary ALIAS CppGeometryLibrary)

target_include_directories(CppGeometryLibrary INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(CppGeometryLibrary INTERFACE cxx_std_17)

# Tests (optional, enabled when this is the top-level project)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
    if(BUILD_TESTING)
        find_package(GTest REQUIRED)
        add_executable(cpp_geometry_tests tests/Tests.cpp)
        target_link_libraries(cpp_geometry_tests
            PRIVATE CppGeometryLibrary GTest::GTest GTest::Main
        )
        add_test(NAME cpp_geometry_tests COMMAND cpp_geometry_tests)
    endif()
endif()

# Installation
include(GNUInstallDirs)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(TARGETS CppGeometryLibrary
    EXPORT CppGeometryLibraryTargets
)
install(EXPORT CppGeometryLibraryTargets
    FILE CppGeometryLibraryTargets.cmake
    NAMESPACE CppGeometryLibrary::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppGeometryLibrary
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    cmake/CppGeometryLibraryConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/CppGeometryLibraryConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppGeometryLibrary
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CppGeometryLibraryConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/CppGeometryLibraryConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/CppGeometryLibraryConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/CppGeometryLibrary
)
