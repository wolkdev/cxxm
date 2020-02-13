
#include "tools/file_tools.hpp"

#include "tools/parsing_tools.hpp"

#include <iostream>
#include <fstream>

std::fs::path to_unix_path(const std::fs::path& _path)
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

std::fs::path to_win_path(const std::fs::path& _path)
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

bool path_contains_base(const std::fs::path& _path, const std::fs::path& _base)
{
    std::fs::path temp = _path;

    while (temp != _base && temp.has_filename())
    {
        temp = temp.parent_path();
    }
    
    return temp == _base;
}

std::fs::path get_path_diff(const std::fs::path& _path, const std::fs::path& _base)
{
    std::fs::path temp = _path;
    std::fs::path diff;

    while (temp != _base && temp.has_filename())
    {
        diff = temp.filename() / diff;
        temp = temp.parent_path();
    }
    
    return diff;
}

bool find_file_in_hierarchy(const std::string& _fileName, std::fs::path& _path)
{
    std::fs::path path = std::fs::current_path() / _fileName;

    do
    {
        path = path.parent_path();

        if (std::fs::exists(path / _fileName))
        {
            _path = path / _fileName;
            return true;
        }

    } while (path.has_filename());

    return false;
}

std::string file_read_all_text(const std::fs::path& _filePath)
{
    std::ifstream file(_filePath);
    std::string text;

    if (file.is_open())
    {
        while (!file.eof())
        {
            text += file.get();
        }

        // pop eof char
        text.pop_back();

        file.close();
    }

    return text;
}

bool file_write_all_text(const std::fs::path& _filePath, const std::string& _text)
{
    std::ofstream file(_filePath);

    if (file.is_open())
    {
        file << _text;

        file.close();

        return true;
    }

    return false;
}

void clear_empty_directories(const std::fs::path& _path)
{
    std::fs::path path = _path;
    std::fs::path pathToRemove;
    std::error_code error;

    while (std::fs::is_empty(path, error))
    {
        if (error)
        {
            return;
        }

        pathToRemove = path;
        path = path.parent_path();
    }
    
    if (!pathToRemove.empty())
    {
        std::fs::remove(pathToRemove);
    }
}

bool create_file(const std::fs::path& _path, const std::string& _text)
{
    const std::fs::path& dir = _path.parent_path();

    return (std::fs::is_directory(dir)
        || std::fs::create_directories(dir))
        && file_write_all_text(_path, _text);
}

bool remove_file(const std::fs::path& _path)
{
    if (std::fs::exists(_path) && std::fs::remove(_path))
    {
        clear_empty_directories(_path.parent_path());

        return true;
    }

    return false;
}

bool move_file(
    const std::fs::path& _from,
    const std::fs::path& _to,
    const std::string& _textFrom,
    const std::string& _textTo)
{
    const std::fs::path& dir = _to.parent_path();

    if (std::fs::exists(_from)
        && (std::fs::is_directory(dir)
        || std::fs::create_directories(dir)))
    {
        try
        {
            std::fs::rename(_from, _to);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;

            return false;
        }
        
        clear_empty_directories(_from.parent_path());

        std::string text = file_read_all_text(_to);
        replace_all(text, _textFrom, _textTo);
        file_write_all_text(_to, text);

        return true;
    }

    return false;
}