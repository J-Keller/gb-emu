cmake_minimum_required(VERSION 3.23)
project(gb-emu)

set(CMAKE_CXX_STANDARD 23)

add_executable(gb-emu main.cpp)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(gb-emu ${SDL2_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(gb-emu Threads::Threads)