cmake_minimum_required(VERSION 3.21)
project(FractalApp CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3 IMPORTED_TARGET)
find_package(OpenGL REQUIRED)

add_executable(FractalApp 
    src/main.cpp 
    glad/src/glad.cpp
)

target_include_directories(FractalApp PRIVATE glad/include)

target_link_libraries(FractalApp PRIVATE 
    PkgConfig::SDL3 
    OpenGL::GL 
    ${CMAKE_DL_LIBS}
)

configure_file(lost.bmp ${CMAKE_CURRENT_BINARY_DIR}/lost.bmp COPYONLY)
add_custom_command(TARGET FractalApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/shaders
        ${CMAKE_CURRENT_BINARY_DIR}/shaders
)
