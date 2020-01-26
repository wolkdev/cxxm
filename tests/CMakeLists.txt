project(TEST)

set(CXXM_TEST_SOURCES
#CXXM_BEGIN
sources/main.cpp)
#CXXM_BEGIN

include_directories(includes)
add_executable(TEST ${CXXM_TEST_SOURCES})

target_link_libraries(TEST libcxxm)