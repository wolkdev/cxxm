
#include "project.hpp"
#include "cmd.hpp"

#include <filesystem>
#include <iostream>

void add(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        try
        {
            std::filesystem::path path = _args[0].string;

            if (_args[0].have_option("local"))
            {
                path = proj.local_to_project_path(path);
                std::cout << path << std::endl;
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
            }
            else
            {
                std::cerr << "Cannot add class" << std::endl;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

void move(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToMove(_args[0].string);
        project::cxxclass classMoved(_args[1].string);

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
        }
        else
        {
            std::cerr << "Cannot rename class" << std::endl;
        }
    }
}

void remove(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToRemove(_args[0].string);

        try
        {
            proj.delete_header(classToRemove);
            proj.delete_source(classToRemove);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }

        if (proj.remove_class(classToRemove))
        {
            proj.save();
        }
        else
        {
            std::cerr << "Cannot remove class" << std::endl;
        }
    }
}

int main(int _argc, char const* _argv[])
{
    // TODO :
    // - polish all command + catch error in cmd class
    // - Doc in the README.md

    std::cout << "I' here bitch !" << std::endl;
    std::cout << _argc << std::endl;

    if (_argc > 1)
    {
        std::string command = _argv[1];
        int argc = _argc - 2;
        const char** argv = (argc > 0 ? &_argv[2] : nullptr);

        cmd commander;

        cmd::data addCMD("add", &add);
        addCMD.add_options({ "local", "global" });
        addCMD.add_options({ "header" });

        cmd::data moveCMD("move", &move);
        moveCMD.add_options({ "local", "global" });

        cmd::data removeCMD("remove", &remove);
        removeCMD.add_options({ "local", "global" });

        commander.add(addCMD);
        commander.add(moveCMD);
        commander.add(removeCMD);
        
        commander.execute(command, argc, argv);
    }

    return 0;
}