
#include "commands.hpp"

int main(int _argc, char const* _argv[])
{
    // TODO :
    // - command line completion

    if (_argc > 1)
    {
        std::string command = _argv[1];
        int argc = _argc - 2;
        const char** argv = (argc > 0 ? &_argv[2] : nullptr);

        cmd commander;

        cmd::data initCMD("init", &init, 1, 1);

        cmd::data addCMD("add", &add, 1, 1);
        addCMD.add_options({ "local", "global" });
        addCMD.add_options({ "header-only" });

        cmd::data moveCMD("move", &move, 2, 2);
        moveCMD.add_options({ "local", "global" });

        cmd::data removeCMD("remove", &remove, 1, 1);
        removeCMD.add_options({ "local", "global" });

        commander.add(initCMD);
        commander.add(addCMD);
        commander.add(moveCMD);
        commander.add(removeCMD);
        
        commander.execute(command, argc, argv);
    }

    std::cout << std::endl;

    return 0;
}