cmake_minimum_required(VERSION 3.10)

# Set the project name and the C++ standard
project(SFMLApp VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find SMFL package
find_package(SFML COMPONENTS graphics window system REQUIRED)
# Add the executable
add_executable(SFMLApp src/main.cpp)
# Link SFML libraries
target_link_libraries(SFMLApp sfml-graphics)
