
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

        const std::filesystem::path& cmakepath = path / "CMakeList.txt";

        if (std::filesystem::exists(cmakepath))
        {
            return cmakepath;
        }
    }
    while (path.has_filename());

    return path;
}

int main(int _argc, char const* _argv[])
{
    const std::filesystem::path& cmakepath = GetCMakeFile();

    if (cmakepath.has_filename())
    {
        std::cout << cmakepath << std::endl;
    }
    
    if (_argc > 1)
    {
        std::string cmd = _argv[0];
    }

    return 0;
}