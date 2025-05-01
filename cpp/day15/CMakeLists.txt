# add_executable(Day15 main.cpp)
add_executable(Day15 main2.cpp)

target_link_libraries(Day15 PUBLIC compiler_flags utils)

configure_file(../../inputs/day15.txt ${CMAKE_CURRENT_BINARY_DIR}/day15.txt COPYONLY)
configure_file(../../inputs/day15_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day15_test.txt COPYONLY)
