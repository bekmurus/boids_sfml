cmake_minimum_required(VERSION 3.10)

# Set the project name and the C++ standard
project(BoidSim VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SFML
find_package(SFML COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(boids src/main.cpp)

# Add include directory using target_include_directories
target_include_directories(boids PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link SFML
target_link_libraries(boids sfml-graphics)

message(STATUS "CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
