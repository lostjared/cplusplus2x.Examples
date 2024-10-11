cmake_minimum_required(VERSION 3.10)
project(MatrixRainApp)
# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Locate SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
# Include SDL2 and SDL2_ttf directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})
# Add the executable
add_executable(MatrixRainApp matrix.cpp)

# Link SDL2 and SDL2_ttf libraries
target_link_libraries(MatrixRainApp ${SDL2_LIBRARIES} SDL2_ttf)