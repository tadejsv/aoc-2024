# add_executable(Day13 main.cpp)
# add_executable(Day13 main2.cpp)
add_executable(Day13 main_extra.cpp)

target_link_libraries(Day13 PUBLIC compiler_flags utils Eigen3::Eigen)

configure_file(../../inputs/day13.txt ${CMAKE_CURRENT_BINARY_DIR}/day13.txt COPYONLY)
configure_file(../../inputs/day13_test.txt ${CMAKE_CURRENT_BINARY_DIR}/day13_test.txt COPYONLY)
configure_file(../../inputs/day13_extra.txt ${CMAKE_CURRENT_BINARY_DIR}/day13_extra.txt COPYONLY)
