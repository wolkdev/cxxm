
#include "class_manager.hpp"

int main(int _argc, char const* _argv[])
{   
    // TODO :
    // - maybe command manager
    // - add/remove header only
    // - "rename" command
    // - Doc in the README.md

    if (_argc > 1)
    {
        std::string cmd = _argv[1];

        if (cmd == "init")
        {
            if (_argc > 2)
            {
                std::string projectName = _argv[2];
                init(projectName);
            }
        }
        else if (cmd == "add")
        {
            if (_argc > 2)
            {
                std::string className = _argv[2];
                add_class(className);
            }
        }
        else if (cmd == "remove")
        {
            if (_argc > 2)
            {
                std::string className = _argv[2];
                remove_class(className);
            }
        }
    }

    return 0;
}