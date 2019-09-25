
#include <iostream>
#include <fstream>
#include <string>

#include <filesystem>

std::filesystem::path GetCMakeFile()
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

void init(const std::string& _projectName)
{
    std::filesystem::create_directory("includes");
    std::filesystem::create_directory("sources");

    std::ofstream source("sources/main.cpp");

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

std::string peek_word(std::istream& _istream)
{
    std::streampos startPos = _istream.tellg();

    std::string word;
    char c;

    while ((c = _istream.get()) > 32)
    {
        word += c;
    }

    _istream.seekg(startPos);

    return word;
}

void skip_word(std::istream& _istream)
{
    while (_istream.peek() > 32)
    {
        _istream.get();
    }
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

void add_class(const std::string& _className)
{
    const std::filesystem::path& cmakepath = GetCMakeFile();

    if (cmakepath.has_filename()) // cmake file found
    {
        std::string startPart;
        std::string cxxmPart;
        std::string endPart;

        get_cmake_parts(cmakepath, startPart, cxxmPart, endPart);

        size_t pos = cxxmPart.find(')');

        if (pos != std::string::npos)
        {
            cxxmPart.insert(pos, "\nsources/" + _className + ".cpp");

            save_cmake_file(cmakepath, startPart, cxxmPart, endPart);
        }

        std::cout << cmakepath << std::endl;
    }
}

int main(int _argc, char const* _argv[])
{   
    if (_argc > 1)
    {
        std::string cmd = _argv[1];

        if (cmd == "init")
        {
            if (_argc > 2)
            {
                std::string projectName = _argv[2];
                init(projectName);
            }
        }
        else if (cmd == "add")
        {
            if (_argc > 2)
            {
                std::string className = _argv[2];
                add_class(className);
            }
        }
    }

    return 0;
}