#ifndef COMMANDS__HPP
#define COMMANDS__HPP

#include <cmd/cmd.hpp>

#include "project.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

COMMAND(init, 1, 1, cmd::option_container({ { "tests" } }), "")
{
    project::create_new(_args[0].string, _args[0].have_option("tests"));

    std::cout << "project initialized" << std::endl;
}

COMMAND(add, 1, 1, cmd::option_container({ { "local", "global" }, { "header-only" } }), "")
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

        if (proj.create_header(classToAdd))
        {
            std::cout << "header created : "
                << classToAdd.headerProjectPath << std::endl;
        }
        else
        {
            std::cout << "header creation failed : "
                << classToAdd.headerProjectPath << std::endl;
        }
        
        if (!_args[0].have_option("header-only"))
        {
            if (proj.create_source(classToAdd))
            {
                std::cout << "source created : "
                    << classToAdd.sourceProjectPath << std::endl;

                if (proj.add_class(classToAdd))
                {
                    proj.save();

                    std::cout << "\"" << classToAdd.className << "\""
                        << " class added" << std::endl;
                }
                else
                {
                    std::cout << "cannot add class !" << std::endl;
                }
            }
            else
            {
                std::cout << "source creation failed : "
                    << classToAdd.sourceProjectPath << std::endl;
            }
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

COMMAND(move, 2, 2, cmd::option_container({ { "local", "global" } }), "")
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

        if (proj.move_header(classToMove, classMoved))
        {
            std::cout << "header moved from : "
                << classToMove.headerProjectPath << " to : "
                << classMoved.headerProjectPath << std::endl;
        }
        else
        {
            std::cout << "header move failed from : "
                << classToMove.headerProjectPath << " to : "
                << classMoved.headerProjectPath << std::endl;
        }
        
        if (proj.move_source(classToMove, classMoved))
        {
            std::cout << "source moved from : "
                << classToMove.sourceProjectPath << " to : "
                << classMoved.sourceProjectPath << std::endl;
        }
        else
        {
            std::cout << "source move failed from : "
                << classToMove.sourceProjectPath << " to : "
                << classMoved.sourceProjectPath << std::endl;
        }

        if (proj.rename_class(classToMove, classMoved))
        {
            proj.save();

            std::cout << "\"" << classToMove.className << "\"" << " renamed to "
                << "\"" << classMoved.className << "\"" << std::endl;
        }
        else
        {
            std::cout << "cannot rename class !" << std::endl;
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

COMMAND(remove, 1, 1, cmd::option_container({ { "local", "global" } }), "")
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

        if (proj.delete_header(classToRemove))
        {
            std::cout << "header removed : "
                << classToRemove.headerProjectPath << std::endl;
        }
        else
        {
            std::cout << "header removing failed : "
                << classToRemove.headerProjectPath << std::endl;
        }
        
        if (proj.delete_source(classToRemove))
        {
            std::cout << "source removed : "
                << classToRemove.sourceProjectPath << std::endl;
        }
        else
        {
            std::cout << "source removing failed : "
                << classToRemove.sourceProjectPath << std::endl;
        }

        if (proj.remove_class(classToRemove))
        {
            proj.save();

            std::cout << "\"" << classToRemove.className << "\""
                << " class removed" << std::endl;
        }
        else
        {
            std::cout << "cannot remove class !" << std::endl;
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

#endif // !COMMANDS__HPP