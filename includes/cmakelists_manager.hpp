#ifndef CMAKELISTS_MANAGER__HPP
#define CMAKELISTS_MANAGER__HPP

#include "tools.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::filesystem::path get_cmakelists_path()
{
    std::filesystem::path path = std::filesystem::current_path() / "start";

    do
    {
        path = path.parent_path();

        const std::filesystem::path& cmakepath = path / "CMakeLists.txt";

        if (std::filesystem::exists(cmakepath))
        {
            return cmakepath;
        }
    }
    while (path.has_filename());

    return path;
}

void get_cmake_parts(
    const std::filesystem::path& _cmakepath,
    std::string& _startPart,
    std::string& _cxxmPart,
    std::string& _endPart)
{
    std::ifstream file(_cmakepath);
    
    _startPart.clear();
    _cxxmPart.clear();
    _endPart.clear();

    if (file.is_open())
    {
        bool begined = false;
        bool ended = false;

        while (!file.eof())
        {
            if (!begined)
            {
                _startPart += file.get();

                if (peek_word(file) == "#CXXM_BEGIN")
                {
                    skip_word(file); // skip "#CXXM_BEGIN"

                    begined = true;
                }
            }
            else if (!ended)
            {
                _cxxmPart += file.get();

                if (peek_word(file) == "#CXXM_END")
                {
                    skip_word(file); // skip "#CXXM_END"

                    ended = true;
                }
            }
            else
            {
                _endPart += file.get();
            }
        }

        _endPart.resize(_endPart.length() - 1); // TODO check why weird char at the end
        
        file.close();
    }
}

void save_cmake_file(
    const std::filesystem::path& _cmakepath,
    const std::string& _startPart,
    const std::string& _cxxmPart,
    const std::string& _endPart)
{
    std::ofstream file(_cmakepath);

    if (file.is_open())
    {
        file << _startPart;
        file << "#CXXM_BEGIN";
        file << _cxxmPart;
        file << "#CXXM_END";
        file << _endPart;

        file.close();
    }
}

void create_main_file(const std::string& _sourceDirPath)
{
    std::ofstream source(_sourceDirPath + "/main.cpp");

    if (source.is_open())
    {
        source << "\n";
        source << "#include <iostream>\n";
        source << "\n";
        source << "int main(int _argc, char* _argv[])\n";
        source << "{\n";
        source << "    // I love C++\n";
        source << "}";

        source.close();
    }
}

void create_cmakelists_file(const std::string& _projectName)
{
    std::ofstream file("CMakeLists.txt");

    if (file.is_open())
    {
        file << "cmake_minimum_required(VERSION 3.1)\n";
        file << "\n";
        file << "project(" << _projectName << ")\n";
        file << "\n";
        file << "set(CMAKE_CXX_STANDARD 11)\n";
        file << "\n";
        file << "set(CXXM_SOURCES\n";
        file << "#CXXM_BEGIN\n";
        file << "sources/main.cpp)\n";
        file << "#CXXM_BEGIN\n";
        file << "\n";
        file << "include_directories(includes)\n";
        file << "add_executable(" << _projectName << " ${CXXM_SOURCES})";

        file.close();
    }
}

void init(const std::string& _projectName)
{
    std::filesystem::create_directory("includes");
    std::filesystem::create_directory("sources");

    create_main_file("sources");
    create_cmakelists_file(_projectName);
}

#endif // !CMAKELISTS_MANAGER__HPP