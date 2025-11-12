# add_executable(Day20 main.cpp)
add_executable(Day20 main2.cpp)

target_link_libraries(Day20 PUBLIC compiler_flags utils)

configure_file(../../inputs/day20.txt ${CMAKE_CURRENT_BINARY_DIR}/day20.txt COPYONLY)
configure_file(../../inputs/day20_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day20_test.txt COPYONLY)
