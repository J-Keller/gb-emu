cmake_minimum_required(VERSION 3.23)
project(gb-emu)

set(CMAKE_CXX_STANDARD 23)

add_executable(gb-emu src/main.cpp src/components/video/Display.cpp src/components/video/Display.h src/components/video/VideoIO.cpp src/components/video/VideoIO.h src/components/video/VideoMemory.cpp src/components/video/VideoMemory.h src/components/general/Memory.cpp src/components/general/Memory.h src/components/general/CPU.cpp src/components/general/CPU.h)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
target_link_libraries(gb-emu ${SDL2_LIBRARIES})
find_package(Threads REQUIRED)
target_link_libraries(gb-emu Threads::Threads)