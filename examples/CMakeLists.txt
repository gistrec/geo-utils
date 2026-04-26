foreach(example spherical_util poly_util)
    add_executable(example_${example} ${example}.cpp)
    target_link_libraries(example_${example} PRIVATE CppGeometryLibrary)
endforeach()
