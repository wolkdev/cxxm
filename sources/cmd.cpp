
#include "cmd.hpp"

#include <iostream>
#include <utility>

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

bool cmd::arg::have_option(const std::string& _string) const
{
    for (size_t i = 0; i < options.size(); i++)
    {
        if (options[i].match(_string))
        {
            return true;
        }
    }

    return false;
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

void cmd::data::add_options(const std::vector<std::string>& _options)
{
    options.push_back(_options);
}

void cmd::data::add_options(std::vector<std::string>&& _options)
{
    options.push_back(std::move(_options));
}

void cmd::data::set_help(const std::string& _help)
{
    help = _help;
}

void cmd::data::check_args(const std::vector<arg>& _args)
{
    bool match;

    if (_args.size() < (unsigned)minArgCount)
    {
        throw std::exception("Not enough arguments !");
    }
    else if (maxArgCount > 0 && _args.size() > (unsigned)maxArgCount)
    {
        throw std::exception("Too much arguments !");
    }

    for (size_t i = 0; i < _args.size(); i++)
    {
        for (size_t j = 0; j < _args[i].options.size(); j++)
        {
            match = false;

            if (help != "" && _args[i].options[j].match("help"))
            {
                match = true;
            }

            for (size_t k = 0; k < options.size(); k++)
            {
                for (size_t l = 0; l < options[k].size(); l++)
                {
                    if (_args[i].options[j].match(options[k][l]))
                    {
                        if (match)
                        {
                            throw std::exception("Conflict error !");
                        }

                        match = true;
                    }
                }
            }

            if (!match)
            {
                throw std::exception("No matching options !");
            }
        }
    }
}

void cmd::add(const data& _cmd)
{
    cmds[_cmd.string] = data(_cmd);
}

void cmd::execute(const std::string& _command, int _argc, char const* _argv[])
{
    if (cmds.find(_command) != cmds.end())
    {
        try
        {
            const std::vector<arg>& args = parse_args(_argc, _argv);
            cmds[_command].check_args(args);
            cmds[_command].callback(args);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
}

bool cmd::is_option(std::string _str, int& _index)
{
    if (_str.size() > 0)
    {
        if (_str[0] == '-')
        {
            if (_str.size() > 1)
            {
                if (_str[1] == '-')
                {
                    if (_str.size() > 2)
                    {
                        _index = 2;
                        return true;
                    }
                }
                else
                {
                    _index = 1;
                    return true;
                }
            }
        }
    }

    return false;
}

std::vector<cmd::arg> cmd::parse_args(int _argc, char const* _argv[])
{
    std::vector<arg> args;
    arg current;

    std::string str;
    int index;

    for (int i = 0; i < _argc; i++)
    {
        str = _argv[i];

        if (is_option(str, index))
        {
            current.options.push_back(arg::option(str.substr(index)));
        }
        else
        {
            current.string = str;
            args.push_back(current);
                
            current.clear();
        }
    }

    if (!current.empty())
    {
        args.push_back(current);
    }

    return args;
}