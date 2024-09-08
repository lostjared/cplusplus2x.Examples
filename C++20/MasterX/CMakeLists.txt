cmake_minimum_required(VERSION 3.10)
project(MasterX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall")

# Find SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

# Add executable
add_executable(MasterX src/main.cpp src/window.cpp src/splash.cpp src/dimension.cpp src/terminal.cpp) 

# Link SDL2 and SDL2_ttf
target_link_libraries(MasterX ${SDL2_LIBRARIES} SDL2_ttf)