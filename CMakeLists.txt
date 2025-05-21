
cmake_minimum_required(VERSION 3.31)

project(ex3_assignment2 C)

set(CMAKE_C_STANDARD 11)

add_library(ex3_assignment2_lib function.h function.c gamescreen.c GameManager.c GameManager.h unit.c unit.h )
add_executable(ex3_assignment2 main.c)
target_link_libraries(ex3_assignment2 ex3_assignment2_lib)
