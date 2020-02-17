
#include "tools/parsing_tools.hpp"

std::string peek_word(std::istream& _istream)
{
    std::streampos startPos = _istream.tellg();

    std::string word;
    char c;

    while ((c = _istream.get()) > 32)
    {
        word += c;
    }

    _istream.seekg(startPos);

    return word;
}

void skip_word(std::istream& _istream)
{
    while (_istream.peek() > 32)
    {
        _istream.get();
    }
}

void replace_all(
    std::string& _string,
    const std::string& _from,
    const std::string& _to)
{
    size_t pos = 0;

    do
    {
        pos = _string.find(_from, pos);

        if (pos != std::string::npos)
        {
            _string.replace(pos, _from.length(), _to);
            pos += _to.length();
        }

    } while (pos != std::string::npos && pos < _string.length());
}

std::string replace_all(
    const std::string& _string,
    const std::vector<variable>& _variables)
{
    std::string string = _string;

    for (size_t i = 0; i < _variables.size(); i++)
    {
        replace_all(string, _variables[i].name, _variables[i].value);
    }

    return string;
}