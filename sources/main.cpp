
#include "project.hpp"

#include <filesystem>

void add(const std::filesystem::path& _classRelativePath)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToAdd(_classRelativePath.string());

        proj.create_header(classToAdd);
        proj.create_source(classToAdd);
        proj.add_class(classToAdd);

        proj.save();
    }
}

void remove(const std::filesystem::path& _classRelativePath)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToRemove(_classRelativePath.string());

        proj.delete_header(classToRemove);
        proj.delete_source(classToRemove);
        proj.remove_class(classToRemove);

        proj.save();
    }
}

int main(int _argc, char const* _argv[])
{   
    // TODO :
    // - maybe command manager
    // - add/remove header only
    // - "rename" command
    // - Doc in the README.md

    if (_argc > 1)
    {
        std::string cmd = _argv[1];

        if (cmd == "init")
        {
            if (_argc > 2)
            {
                std::string projectName = _argv[2];
                project::create_new(projectName);
            }
        }
        else if (cmd == "add")
        {
            if (_argc > 2)
            {
                std::string className = _argv[2];
                add(className);
            }
        }
        else if (cmd == "remove")
        {
            if (_argc > 2)
            {
                std::string className = _argv[2];
                remove(className);
            }
        }
    }

    return 0;
}