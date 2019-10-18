#ifndef TOOLS__HPP
#define TOOLS__HPP

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>

inline std::string peek_word(std::istream& _istream)
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

inline void skip_word(std::istream& _istream)
{
    while (_istream.peek() > 32)
    {
        _istream.get();
    }
}

inline std::filesystem::path to_unix_path(const std::filesystem::path& _path)
{
    std::string pathStr = _path.string();

    for (size_t i = 0; i < pathStr.length(); i++)
    {
        if (pathStr[i] == '\\')
        {
            pathStr[i] = '/';
        }
    }

    return pathStr;
}

inline std::filesystem::path to_win_path(const std::filesystem::path& _path)
{
    std::string pathStr = _path.string();
    
    for (size_t i = 0; i < pathStr.length(); i++)
    {
        if (pathStr[i] == '/')
        {
            pathStr[i] = '\\';
        }
    }

    return pathStr;
}

inline std::string file_read_all_text(const std::filesystem::path& _filePath)
{
    std::ifstream file(_filePath);
    std::string text;

    if (file.is_open())
    {
        while (!file.eof())
        {
            text += file.get();
        }

        file.close();
    }

    return text;
}

inline void file_write_all_text(const std::filesystem::path& _filePath, const std::string& _text)
{
    std::ofstream file(_filePath);

    if (file.is_open())
    {
        file << _text;

        file.close();
    }
}

inline void replace_all(std::string& _string, const std::string& _from, const std::string& _to)
{
    size_t pos = 0;

    do
    {
        pos = _string.find(_from);

        if (pos != std::string::npos)
        {
            _string.replace(pos, _from.length(), _to);
            pos += _to.length();
        }

    } while (pos != std::string::npos && pos < _string.length());
}

#endif // !TOOLS__HPP