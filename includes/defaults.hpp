#ifndef DEFAULTS__HPP
#define DEFAULTS__HPP

#define CMAKE_LISTS_DEFAULT_FILE                \
    "cmake_minimum_required(VERSION 3.1)\n"     \
    "\n"                                        \
    "project(${NAME})\n"                        \
    "\n"                                        \
    "include_directories(includes)\n"           \
    "\n"                                        \
    "set(CXXM_SOURCES\n"                        \
    "#CXXM_BEGIN\n"                             \
    "sources/main.cpp)\n"                       \
    "#CXXM_END\n"                               \
    "\n"                                        \
    "add_executable(${NAME} ${CXXM_SOURCES})"

#define CMAKE_LISTS_WITH_TESTS_DEFAULT_FILE         \
    "cmake_minimum_required(VERSION 3.1)\n"         \
    "\n"                                            \
    "project(${MAME})\n"                            \
    "project(lib${MAME})\n"                         \
    "\n"                                            \
    "include_directories(includes)\n"               \
    "\n"                                            \
    "add_subdirectory(tests)\n"                     \
    "\n"                                            \
    "set(CXXM_SOURCES\n"                            \
    "#CXXM_BEGIN\n"                                 \
    ")\n"                                           \
    "#CXXM_END\n"                                   \
    "\n"                                            \
    "add_library(lib${MAME} ${CXXM_SOURCES})\n"     \
    "add_executable(${MAME} sources/main.cpp)\n"    \
    "target_link_libraries(${MAME} lib${MAME})\n"

#define TESTS_CMAKE_LISTS_DEFAULT_FILE              \
    "project(TESTS)\n"                              \
    "\n"                                            \
    "include_directories(includes)\n"               \
    "\n"                                            \
    "set(CXXM_TESTS_SOURCES\n"                      \
    "#CXXM_BEGIN\n"                                 \
    "sources/main.cpp)\n"                           \
    "#CXXM_END\n"                                   \
    "\n"                                            \
    "add_executable(TESTS ${CXXM_TESTS_SOURCES})"   \
    "target_link_libraries(TESTS lib${MAME})"

#define MAIN_DEFAULT_FILE                   \
    "\n"                                    \
    "#include <iostream>\n"                 \
    "\n"                                    \
    "int main(int _argc, char* _argv[])\n"  \
    "{\n"                                   \
    "    // I love C++\n"                   \
    "}"

#define HEADER_DEFAULT_FILE             \
    "#ifndef ${DEFINITION_NAME}\n"      \
    "#define ${DEFINITION_NAME}\n"      \
    "\n"                                \
    "class ${CLASS_NAME}\n"             \
    "{\n"                               \
    "    \n"                            \
    "};\n"                              \
    "\n"                                \
    "#endif // !${DEFINITION_NAME}"

#define SOURCE_DEFAULT_FILE             \
    "\n"                                \
    "#include \"${INCLUDE_PATH}\"\n"    \
    "\n"

#endif // !DEFAULTS__HPP