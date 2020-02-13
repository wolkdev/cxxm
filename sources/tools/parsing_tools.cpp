
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

std::string& replace_all(
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

    return _string;
}

std::string replace_all(
    const std::string& _string,
    const std::vector<variable>& _variables)
{
    std::string string = _string;
    size_t pos = 0;
    int index = 0;

    do
    {
        pos = std::string::npos;

        for (size_t i = 0; i < _variables.size(); i++)
        {
            size_t currentPos = string.find(_variables[i].name, pos);

            if (currentPos != std::string::npos
                && (currentPos < pos || pos == std::string::npos))
            {
                pos = currentPos;
                index = i;
            }
        }

        if (pos != std::string::npos)
        {
            const variable& v = _variables[index];
            
            string.replace(pos, v.name.length(), v.value);
            pos += v.value.length();
        }

    } while (pos != std::string::npos && pos < string.length());

    return _string;
}