
#include "commands.hpp"

int main(int _argc, char const* _argv[])
{
    // TODO :
    // - command line completion
    // - use ctest for tests in cmakelists
    // - remove --recursive option

    if (_argc > 1)
    {
        std::string command = _argv[1];
        int argc = _argc - 2;
        const char** argv = (argc > 0 ? &_argv[2] : nullptr);
        
        std::cout << std::endl;
        cmd::execute(command, argc, argv);
        std::cout << std::endl;
    }

    return 0;
}