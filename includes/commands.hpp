#ifndef COMMANDS__HPP
#define COMMANDS__HPP

#include "project.hpp"
#include "cmd.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

inline void init(const std::vector<cmd::arg>& _args)
{
    project::create_new(_args[0].string);
}

inline void add(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        std::filesystem::path path = _args[0].string;

        if (_args[0].have_option("local"))
        {
            path = proj.local_to_project_path(path);
        }

        project::cxxclass classToAdd(path.string());

        proj.create_header(classToAdd);

        if (!_args[0].have_option("header"))
        {
            proj.create_source(classToAdd);
        }

        if (proj.add_class(classToAdd))
        {
            proj.save();

            std::cout << "Class correctly added in the project !" << std::endl;
        }
        else
        {
            std::cout << "Cannot add class !" << std::endl;
        }
    }
}

inline void move(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        std::filesystem::path pathToMoved = _args[0].string;

        if (_args[0].have_option("local"))
        {
            pathToMoved = proj.local_to_project_path(pathToMoved);
        }

        std::filesystem::path pathMoved = _args[1].string;

        if (_args[1].have_option("local"))
        {
            pathMoved = proj.local_to_project_path(pathMoved);
        }

        project::cxxclass classToMove(pathToMoved.string());
        project::cxxclass classMoved(pathMoved.string());

        try
        {
            proj.move_header(classToMove, classMoved);
            proj.move_source(classToMove, classMoved);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        if (proj.rename_class(classToMove, classMoved))
        {
            proj.save();

            std::cout << "Class correctly moved in the project !" << std::endl;
        }
        else
        {
            std::cout << "Cannot rename class !" << std::endl;
        }
    }
}

inline void remove(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        std::filesystem::path path = _args[0].string;

        if (_args[0].have_option("local"))
        {
            path = proj.local_to_project_path(path);
        }

        project::cxxclass classToRemove(path.string());

        proj.delete_header(classToRemove);
        proj.delete_source(classToRemove);

        if (proj.remove_class(classToRemove))
        {
            proj.save();

            std::cout << "Class correctly removed from the project !" << std::endl;
        }
        else
        {
            std::cout << "Cannot remove class !" << std::endl;
        }
    }
}

#endif // !COMMANDS__HPP