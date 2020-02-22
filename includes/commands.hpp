#ifndef COMMANDS__HPP
#define COMMANDS__HPP

#include <cmd/cmd.hpp>

#include "project.hpp"

#include "tools/file_tools.hpp"
#include "tools/parsing_tools.hpp"

#include "defaults.hpp"

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

    create_file("sources/main.cpp", get_file_content("main"));

    if (_args[0].have_option("tests"))
    {
        std::fs::create_directory("tests");
        std::fs::create_directory("tests/includes");
        std::fs::create_directory("tests/sources");

        create_file("tests/sources/main.cpp",
            get_file_content("main", "tests"));

        create_file("tests/CMakeLists.txt", replace_all(
            get_file_content("tests-cmakelists"), variables));

        create_file("CMakeLists.txt", replace_all(
            get_file_content("cmakelists-with-tests"), variables));

        std::cout << "project initialized with tests" << std::endl;
    }
    else
    {
        create_file("CMakeLists.txt", replace_all(
            get_file_content("cmakelists"), variables));

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

        if (!proj.have_class(classToAdd))
        {
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
            std::cout << "class already exists in the project !" << std::endl;
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

COMMAND
(
    dump, 1, 2,
    cmd::option_container(),
    ""
)
{
    if (_args[0].string == "defaults")
    {
        std::fs::path dir;

        if (_args.size() == 2)
        {
            dir = _args[1].string;
        }
        else
        {
            dir = get_home_path() / ".cxxm" / "files";
        }

        for (auto a : defaults::map)
        {
            const std::fs::path path = dir / (a.first + ".txt");

            if (create_file(path, a.second))
            {
                std::cout << "file created at path : " << path << std::endl;
            }
            else
            {
                std::cout << "fail to create file at path : " << path << std::endl;
            }
        }
    }
}

#endif // !COMMANDS__HPP