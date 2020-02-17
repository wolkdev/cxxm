#ifndef CXXCLASS__HPP
#define CXXCLASS__HPP

#include "tools/file_tools.hpp"
#include "tools/parsing_tools.hpp"

class project;

class cxxclass
{
    public:

    std::string className;
    std::fs::path projectDir;

    std::fs::path headerRelativePath;
    std::fs::path sourceRelativePath;
    std::fs::path headerProjectPath;
    std::fs::path sourceProjectPath;

    std::vector<variable> variables;

    cxxclass(const project& _project, const std::string& _name);

    bool create_header();
    bool create_source();
    bool remove_header();
    bool remove_source();
    bool move_header(const cxxclass& _other);
    bool move_source(const cxxclass& _other);

    std::string definition_name() const;
};

#endif // !CXXCLASS__HPP