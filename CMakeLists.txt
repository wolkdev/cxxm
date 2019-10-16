cmake_minimum_required(VERSION 3.1)
project(cxxm)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
#CXXM_BEGIN
sources/main.cpp
sources/project.cpp
sources/cmd.cpp)
#CXXM_END

include_directories(includes)

add_executable(cxxm ${SOURCES})