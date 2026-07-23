cmake_minimum_required(VERSION 3.16)

project(eigen_test LANGUAGES CXX)

find_package(Eigen3 REQUIRED NO_MODULE)

add_executable(eigen_test main.cpp)
target_compile_features(eigen_test PRIVATE cxx_std_23)
target_link_libraries(eigen_test PRIVATE Eigen3::Eigen)
