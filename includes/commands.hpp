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
        { "directory" },
        { "tests" }
    }),

    "Init a new project with a basic CMakeLists.txt in the current directory\n"
    "\n"
    "Options :\n"
    "\n"
    "    --directory -d : Init the project in a new created directory\n"
    "\n"
    "    --tests -t : Init the project with a tests sub-project"
)
{
    std::vector<variable> variables = { { "${NAME}", _args[0].string } };

    std::fs::path directory = std::fs::current_path();

    if (_args[0].have_option("directory"))
    {
        std::fs::create_directory(_args[0].string);
        directory = directory / _args[0].string;
    }

    std::fs::create_directory(directory / "includes");
    std::fs::create_directory(directory / "sources");

    create_file(directory / "sources/main.cpp", get_file_content("main"));

    if (_args[0].have_option("tests"))
    {
        std::fs::create_directory(directory / "tests");
        std::fs::create_directory(directory / "tests/includes");
        std::fs::create_directory(directory / "tests/sources");

        create_file(directory / "tests/sources/main.cpp",
            get_file_content("main", "tests"));

        create_file(directory / "tests/CMakeLists.txt", replace_all(
            get_file_content("tests-cmakelists"), variables));

        create_file(directory / "CMakeLists.txt", replace_all(
            get_file_content("cmakelists-with-tests"), variables));

        std::cout << "project initialized with tests in directory : "
            << directory << std::endl;
    }
    else
    {
        create_file(directory / "CMakeLists.txt", replace_all(
            get_file_content("cmakelists"), variables));

        std::cout << "project initialized in directory : "
            << directory << std::endl;
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
    
    "Create class header and source files and add "
    "the source to the CMakeLists.txt file\n"
    "\n"
    "* Note that cxxm will automatically create directories "
    "that don't already exists\n"
    "\n"
    "Options :\n"
    "\n"
    "    --header-only : Only create the header file\n"
    "\n"
    "    --global -g : Path is relative to the sources / includes folder "
    "(that's the default behavior)\n"
    "\n"
    "    --local -l : Path is local to your current position. "
    "So you need to be in the sources or includes folder "
    "otherwise the command fail"
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
    ({
        { "recursive" },
        { "local", "global" }
    }),
    
    "Remove class header and source files and "
    "remove the source from the CMakeLists.txt file\n"
    "\n"
    "* Note that cxxm will automatically delete empty directories "
    "after a file remove\n"
    "\n"
    "Options :\n"
    "\n"
    "    --recursive -r : Remove the removed header's includes "
    "in all project files"
    "\n"
    "    --global -g : Path is relative to the sources / includes folder "
    "(that's the default behavior)\n"
    "\n"
    "    --local -l : Path is local to your current position. "
    "So you need to be in the sources or includes folder "
    "otherwise the command fail"
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

        if (_args[0].have_option("recursive"))
        {
            classToRemove.remove_all_includes(
                proj.get_all_project_files());
        }

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
        { "recursive" },
        { "local", "global" }
    }),
    
    "Move class header and source files and "
    "rename the source in the CMakeLists.txt file.\n"
    "also rename the include reference in the source and "
    "the header's definition name\n"
    "\n"
    "* Note that cxxm will automatically create directories "
    "that don't already exists\n"
    "and delete empty directories after a file move\n"
    "\n"
    "Options :\n"
    "\n"
    "    --recursive -r : Replace the moved header's includes "
    "in all project files"
    "\n"
    "    --global -g : Path is relative to the sources / includes folder "
    "(that's the default behavior)\n"
    "\n"
    "    --local -l : Path is local to your current position. "
    "So you need to be in the sources or includes folder "
    "otherwise the command fail"
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
            && classToMove.move_source(classMoved))
        {
            if (_args[0].have_option("recursive"))
            {
                classToMove.replace_all_includes(
                    proj.get_all_project_files(), classMoved);
            }

            if (proj.rename_class(classToMove, classMoved))
            {
                proj.save();
            }
        }
    }
    else
    {
        std::cout << "cannot find the CMakeLists.txt !" << std::endl;
    }
}

COMMAND
(
    dump, 1, 1,

    cmd::option_container(),

    "Dump default contents files in the <user>/.cxxm/files directory\n"
    "so you can overwrite the content used by cxxm to create files"
)
{
    if (_args[0].string == "defaults")
    {
        std::fs::path dir = get_home_path() / ".cxxm" / "files";

        for (const auto& a : defaults::map)
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