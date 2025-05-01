# add_executable(Day14 main.cpp)
add_executable(Day14 main2.cpp)

target_link_libraries(Day14 PUBLIC compiler_flags utils)

configure_file(../../inputs/day14.txt ${CMAKE_CURRENT_BINARY_DIR}/day14.txt COPYONLY)
configure_file(../../inputs/day14_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day14_test.txt COPYONLY)
