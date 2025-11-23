add_executable(Day25 main.cpp)

target_link_libraries(Day25 PUBLIC compiler_flags utils)

configure_file(../../inputs/day25.txt ${CMAKE_CURRENT_BINARY_DIR}/day25.txt COPYONLY)
