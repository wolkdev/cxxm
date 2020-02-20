#ifndef DEFAULT_FILE_CONTENTS__HPP
#define DEFAULT_FILE_CONTENTS__HPP

#include <string>
#include <unordered_map>

static std::unordered_map<std::string, std::string> default_file_contents =
{
    {
        "cmakelists",

        "cmake_minimum_required(VERSION 3.1)\n"
        "\n"
        "project(${NAME})\n"
        "\n"
        "include_directories(includes)\n"
        "\n"
        "set(CXXM_SOURCES\n"
        "#CXXM_BEGIN\n"
        "sources/main.cpp)\n"
        "#CXXM_END\n"
        "\n"
        "add_executable(${NAME} ${CXXM_SOURCES})"
    },
    {
        "cmakelists-with-tests",
        
        "cmake_minimum_required(VERSION 3.1)\n"
        "\n"
        "project(${NAME})\n"
        "project(lib${NAME})\n"
        "\n"
        "include_directories(includes)\n"
        "\n"
        "add_subdirectory(tests)\n"
        "\n"
        "set(CXXM_SOURCES\n"
        "#CXXM_BEGIN\n"
        ")\n"
        "#CXXM_END\n"
        "\n"
        "add_library(lib${NAME} ${CXXM_SOURCES})\n"
        "add_executable(${NAME} sources/main.cpp)\n"
        "target_link_libraries(${NAME} lib${NAME})"
    },
    {
        "tests-cmakelists",
        
        "project(TESTS)\n"
        "\n"
        "include_directories(includes)\n"
        "\n"
        "set(CXXM_TESTS_SOURCES\n"
        "#CXXM_BEGIN\n"
        "sources/main.cpp)\n"
        "#CXXM_END\n"
        "\n"
        "add_executable(TESTS ${CXXM_TESTS_SOURCES})\n"
        "target_link_libraries(TESTS lib${NAME})"
    },
    {
        "main",
        
        "\n"
        "#include <iostream>\n"
        "\n"
        "int main(int _argc, char* _argv[])\n"
        "{\n"
        "    // I love C++\n"
        "}"
    },
    {
        "header",
        
        "#ifndef ${DEFINITION_NAME}\n"
        "#define ${DEFINITION_NAME}\n"
        "\n"
        "class ${CLASS_NAME}\n"
        "{\n"
        "    \n"
        "};\n"
        "\n"
        "#endif // !${DEFINITION_NAME}"
    },
    {
        "source",
        
        "\n"
        "#include \"${INCLUDE_PATH}\"\n"
        "\n"
    },
};

#endif // !DEFAULT_FILE_CONTENTS__HPP