# add_executable(Day21 main.cpp)
add_executable(Day21 main2.cpp)

target_link_libraries(Day21 PUBLIC compiler_flags utils)

configure_file(../../inputs/day21.txt ${CMAKE_CURRENT_BINARY_DIR}/day21.txt COPYONLY)
configure_file(../../inputs/day21_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day21_test.txt COPYONLY)
