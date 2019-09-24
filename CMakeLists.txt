cmake_minimum_required(VERSION 3.1)
project(cxxm)

set(CMAKE_CXX_STANDARD 11)

set(SOURCES sources/main.cpp)

include_directories(includes)

add_executable(gxxnet ${SOURCES})