cmake_minimum_required(VERSION 3.10)
project(SortingVisualizer)

set(CMAKE_CXX_STANDARD 11)

# Add the main executable
add_executable(SortingVisualizer main.cpp)

# Find SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Link SDL2
target_link_libraries(SortingVisualizer ${SDL2_LIBRARIES})
