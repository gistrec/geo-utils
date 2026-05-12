-- Smoke test for an installed geo-utils-cpp package via xrepo.
-- Mirrors CMakeLists.txt: declare the dependency, link, build, run main.cpp.
-- Used by .github/workflows/xrepo.yml.

set_languages("c++17")

add_requires("geo-utils-cpp")

target("smoke")
    set_kind("binary")
    add_files("main.cpp")
    add_packages("geo-utils-cpp")
