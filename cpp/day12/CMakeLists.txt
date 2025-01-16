# add_executable(Day12 main.cpp)
add_executable(Day12 main2.cpp)

target_link_libraries(Day12 PUBLIC compiler_flags utils Eigen3::Eigen)

configure_file(../../inputs/day12.txt ${CMAKE_CURRENT_BINARY_DIR}/day12.txt COPYONLY)
configure_file(../../inputs/day12_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day12_test.txt COPYONLY)
