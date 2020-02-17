cmake_minimum_required(VERSION 3.1)


project(libcxxm)
project(cxxm)


set(CMAKE_CXX_STANDARD 17)

include_directories(includes)
include_directories(externs)

set(CXXM_SOURCES
#CXXM_BEGIN
sources/project.cpp
sources/cxxclass.cpp
sources/tools/file_tools.cpp
sources/tools/parsing_tools.cpp)
#CXXM_END


add_subdirectory(tests)

enable_testing()
add_test(NAME unit COMMAND
cmake -S tests -B tests/build &&
cmake --build tests/build)


add_library(libcxxm ${CXXM_SOURCES})
add_executable(cxxm sources/main.cpp)

target_link_libraries(cxxm libcxxm)