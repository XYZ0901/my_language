cmake_minimum_required(VERSION 3.23)
project(my_language C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

add_executable(my_language main.c)

add_subdirectory(chap5-cpp/)