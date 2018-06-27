cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(FinalProject LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp flint_wrapp.cpp flint_wrapp.hpp spirit_parser.cpp)

#flint
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(flint)
target_link_libraries(${PROJECT_NAME} PRIVATE flint::flint)

#gmpz
find_package(gmp)
target_link_libraries(${PROJECT_NAME} PRIVATE gmp::gmp)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

