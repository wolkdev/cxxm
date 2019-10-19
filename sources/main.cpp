
#include "project.hpp"
#include "cmd.hpp"

#include <filesystem>

void add(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToAdd(_args[0].string);

        proj.create_header(classToAdd);
        proj.create_source(classToAdd);

        if (proj.add_class(classToAdd))
        {
            proj.save();
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

        proj.move_header(classToMove, classMoved);
        proj.move_source(classToMove, classMoved);

        if (proj.rename_class(classToMove, classMoved))
        {
            proj.save();
        }
    }
}

void remove(const std::vector<cmd::arg>& _args)
{
    project proj(project::find_directory_in_hierarchy());

    if (proj.valid())
    {
        project::cxxclass classToRemove(_args[0].string);

        proj.delete_header(classToRemove);
        proj.delete_source(classToRemove);

        if (proj.remove_class(classToRemove))
        {
            proj.save();
        }
    }
}

int main(int _argc, char const* _argv[])
{
    // TODO :
    // - add/remove header only
    // - errors/warnings feedback -> cout
    // - handle local and project global path
    // - Doc in the README.md

    if (_argc > 1)
    {
        std::string command = _argv[1];
        int argc = _argc - 2;
        const char** argv = (argc > 0 ? &_argv[2] : nullptr);

        cmd commander;
        commander.add("add", &add);
        commander.add("move", &move);
        commander.add("remove", &remove);
        commander.execute(command, argc, argv);
    }

    return 0;
}