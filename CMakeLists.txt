cmake_minimum_required(VERSION 2.8)

project(math_logic_hw)

include_directories(${PROJECT_SOURCE_DIR}/axe_parser)

add_definitions(-std=c++11 -g)

add_library(common ${PROJECT_SOURCE_DIR}/source/lambda_parser.cpp)

add_executable(hw1 ${PROJECT_SOURCE_DIR}/source/hw1.cpp)
add_executable(tests ${PROJECT_SOURCE_DIR}/source/test.cpp)
# add_executable(hw2 ${PROJECT_SOURCE_DIR}/source/hw2.cpp)
# add_executable(hw3 ${PROJECT_SOURCE_DIR}/source/hw3.cpp)

target_link_libraries(tests common)

target_link_libraries(hw1 common)
# target_link_libraries(hw2 common)
# target_link_libraries(hw3 common)