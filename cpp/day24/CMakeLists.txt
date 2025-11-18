add_executable(Day24 main.cpp)
# add_executable(Day24 main2.cpp)

target_link_libraries(Day24 PUBLIC compiler_flags utils)

configure_file(../../inputs/day24.txt ${CMAKE_CURRENT_BINARY_DIR}/day24.txt COPYONLY)
configure_file(../../inputs/day24_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day24_test.txt COPYONLY)
