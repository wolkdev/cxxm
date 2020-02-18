#ifndef COMMANDS__HPP
#define COMMANDS__HPP

#include <cmd/cmd.hpp>

#include "defaults.hpp"
#include "project.hpp"

#include "tools/file_tools.hpp"
#include "tools/parsing_tools.hpp"

#include <filesystem>
#include <iostream>
#include <vector>

COMMAND
(
    init, 1, 1,
    cmd::option_container
    ({
        { "tests" }
    }),
    ""
)
{
    std::vector<variable> variables = { { "${NAME}", _args[0].string } };

    std::fs::create_directory("includes");
    std::fs::create_directory("sources");

    create_file("sources/main.cpp", MAIN_DEFAULT_FILE);

    if (_args[0].have_option("tests"))
    {
        std::fs::create_directory("tests");
        std::fs::create_directory("tests/includes");
        std::fs::create_directory("tests/sources");

        create_file("tests/sources/main.cpp", MAIN_DEFAULT_FILE);

        create_file("tests/CMakeLists.txt", replace_all(
            TESTS_CMAKE_LISTS_DEFAULT_FILE, variables));

        create_file("CMakeLists.txt", replace_all(
            CMAKE_LISTS_WITH_TESTS_DEFAULT_FILE, variables));

        std::cout << "project initialized with tests" << std::endl;
    }
    else
    {
        create_file("CMakeLists.txt", replace_all(
            CMAKE_LISTS_DEFAULT_FILE, variables));

        std::cout << "project initialized" << std::endl;
    }
}

COMMAND
(
    add, 1, 1,
    cmd::option_container
    ({
        { "local", "global" },
        { "header-only" }
    }),
    ""
)
{
    std::fs::path cmakeListsPath;

    if (find_file_in_hierarchy("CMakeLists.txt", cmakeListsPath))
    {
        project proj(cmakeListsPath);

        const std::fs::path& path = _args[0].have_option("local") ?
            proj.local_to_project_path(_args[0].string) : _args[0].string;

        cxxclass classToAdd(proj, path.string());

        if (_args[0].have_option("header-only"))
        {
            classToAdd.create_header();
        }
        else if (classToAdd.create_header()
            && classToAdd.create_source()
            && proj.add_class(classToAdd))
        {
            proj.save();
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

COMMAND
(
    remove, 1, 1,
    cmd::option_container
    ({ { "local", "global" } }),
    ""
)
{
    std::fs::path cmakeListsPath;

    if (find_file_in_hierarchy("CMakeLists.txt", cmakeListsPath))
    {
        project proj(cmakeListsPath);

        const std::fs::path& path = _args[0].have_option("local") ?
            proj.local_to_project_path(_args[0].string) : _args[0].string;

        cxxclass classToRemove(proj, path.string());

        classToRemove.remove_header();
        classToRemove.remove_source();

        if (proj.remove_class(classToRemove))
        {
            proj.save();
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

COMMAND
(
    move, 2, 2,
    cmd::option_container
    ({
        { "local", "global" }
    }),
    ""
)
{
    std::fs::path cmakeListsPath;

    if (find_file_in_hierarchy("CMakeLists.txt", cmakeListsPath))
    {
        project proj(cmakeListsPath);

        const std::fs::path& path1 = _args[0].have_option("local") ?
            proj.local_to_project_path(_args[0].string) : _args[0].string;
        const std::fs::path& path2 = _args[1].have_option("local") ?
            proj.local_to_project_path(_args[1].string) : _args[1].string;

        cxxclass classToMove(proj, path1.string());
        cxxclass classMoved(proj, path2.string());

        if (classToMove.move_header(classMoved)
            && classToMove.move_source(classMoved)
            && proj.rename_class(classToMove, classMoved))
        {
            proj.save();
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

#endif // !COMMANDS__HPP