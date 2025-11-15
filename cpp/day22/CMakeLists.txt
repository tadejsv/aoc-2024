# add_executable(Day22 main.cpp)
add_executable(Day22 main2.cpp)

target_link_libraries(Day22 PUBLIC compiler_flags utils)

configure_file(../../inputs/day22.txt ${CMAKE_CURRENT_BINARY_DIR}/day22.txt COPYONLY)
configure_file(../../inputs/day22_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day22_test.txt COPYONLY)
