
#include "commands.hpp"

int main(int _argc, char const* _argv[])
{
    // TODO :
    // - command line completion
    // - use ctest for tests in cmakelists
    // - change include on move / remove in all project files
    // - unit tests
    // - dumb defaults function

    if (_argc > 1)
    {
        std::string command = _argv[1];
        int argc = _argc - 2;
        const char** argv = (argc > 0 ? &_argv[2] : nullptr);

        cmd commander;

        commander.add(init_cmd);
        commander.add(add_cmd);
        commander.add(move_cmd);
        commander.add(remove_cmd);
        
        commander.execute(command, argc, argv);
    }

    std::cout << std::endl;

    return 0;
}