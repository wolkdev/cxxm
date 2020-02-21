project(TEST)

set(CXXM_TEST_SOURCES
#CXXM_BEGIN
sources/main.cpp
sources/file_tests.cpp
sources/parsing_tests.cpp)
#CXXM_END

include_directories(includes)
add_executable(TEST ${CXXM_TEST_SOURCES})

target_link_libraries(TEST libcxxm)