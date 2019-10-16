
#include "cmd.hpp"

#include <iostream>

bool cmd::arg::option::match(const std::string& _string) const
{
    if (string.size() == 0 || _string.size() == 0)
    {
        return false;
    }
    else if (string.size() == 1)
    {
        return string[0] == _string[0];
    }
    else
    {
        return string == _string;
    }
}

bool cmd::arg::check_options(const std::vector<std::string>& _options) const
{
    std::vector<int> matcheds;

    for (size_t i = 0; i < _options.size(); i++)
    {
        if (have_option(_options[i]))
        {
            matcheds.push_back(i);
        }
    }

    return matcheds.size() == options.size();
}

bool cmd::arg::have_option(const std::string& _string) const
{
    std::vector<int> matcheds;

    for (size_t i = 0; i < options.size(); i++)
    {
        if (options[i].match(_string))
        {
            matcheds.push_back(i);
        }
    }
    
    if (matcheds.size() <= 1)
    {
        return matcheds.size() == 1;
    }
    else
    {
        std::cout << "Conflict !" << std::endl;
        return false;
    }
}

void cmd::arg::clear()
{
    string.clear();
    options.clear();
}

bool cmd::arg::empty() const
{
    return string == "" && options.empty();
}

void cmd::add(const std::string& _string, function _function)
{
    functions[_string] = _function;
}

void cmd::execute(const std::string& _command, int _argc, char const* _argv[])
{
    if (functions[_command] != nullptr)
    {
        functions[_command](parse_args(_argc, _argv));
    }
}

std::vector<cmd::arg> cmd::parse_args(int _argc, char const* _argv[])
{
    std::vector<arg> args;
    arg current;

    for (int i = 0; i < _argc; i++)
    {
        std::string str(_argv[i]);

        if (str.size() > 0)
        {
            if (str[0] == '-')
            {
                if (str.size() > 1)
                {
                    if (str[1] == '-')
                    {
                        if (str.size() > 2)
                        {
                            current.options.push_back(
                                arg::option(str.substr(2)));
                        }
                        else
                        {
                            
                        }
                    }
                    else
                    {
                        current.options.push_back(
                            arg::option(str.substr(1)));
                    }
                }
                else
                {
                    
                }
            }
            else
            {
                current.string = str;
                args.push_back(current);
                
                current.clear();
            }
        }
    }

    if (!current.empty())
    {
        args.push_back(current);
    }

    return args;
}