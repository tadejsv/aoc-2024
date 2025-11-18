# add_executable(Day23 main.cpp)
add_executable(Day23 main2.cpp)

target_link_libraries(Day23 PUBLIC compiler_flags utils Eigen3::Eigen)

configure_file(../../inputs/day23.txt ${CMAKE_CURRENT_BINARY_DIR}/day23.txt COPYONLY)
configure_file(../../inputs/day23_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day23_test.txt COPYONLY)
