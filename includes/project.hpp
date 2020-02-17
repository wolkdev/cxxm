#ifndef PROJECT__HPP
#define PROJECT__HPP

#include <filesystem>
#include <string>

#include "cxxclass.hpp"

#include "tools/file_tools.hpp"
#include "tools/parsing_tools.hpp"

class project
{
    private:
    std::fs::path dirPath;
    std::fs::path cmakeListsPath;

    bool isValid;

    std::string startPart;
    std::string cxxmPart;
    std::string endPart;

    public:
    project(const std::fs::path& _cmakeListsPath);
    void save();

    bool valid() const;
    std::fs::path directory() const;
    std::fs::path local_to_project_path(const std::fs::path& _path) const;

    bool add_class(const cxxclass& _class);
    bool remove_class(const cxxclass& _class);
    bool rename_class(const cxxclass& _from, const cxxclass& _to);

    private:
    bool parse_cmake_lists();
};

#endif // !PROJECT__HPP