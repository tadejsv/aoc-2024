# add_executable(Day16 main.cpp)
add_executable(Day16 main2.cpp)

target_link_libraries(Day16 PUBLIC compiler_flags utils)

configure_file(../../inputs/day16.txt ${CMAKE_CURRENT_BINARY_DIR}/day16.txt COPYONLY)
configure_file(../../inputs/day16_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day16_test.txt COPYONLY)
