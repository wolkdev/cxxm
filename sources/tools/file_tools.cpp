
#include "tools/file_tools.hpp"

#include "tools/parsing_tools.hpp"

#include "defaults.hpp"

#include <iostream>
#include <fstream>
#include <cstdio>

std::fs::path get_home_path()
{
    #ifdef _WIN32

    return std::string(std::getenv("HOMEDRIVE"))
        + std::string(std::getenv("HOMEPATH"));

    #else

    return std::string(getenv("HOME"));

    #endif
}

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
        if (diff.empty())
        {
            diff = temp.filename();
        }
        else
        {
            diff = temp.filename() / diff;
        }

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
    
    return (dir.empty()
        || std::fs::is_directory(dir)
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

bool move_file(const std::fs::path& _from, const std::fs::path& _to)
{
    const std::fs::path& dir = _to.parent_path();

    if (std::fs::exists(_from)
        && (dir.empty()
        || std::fs::is_directory(dir)
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

        return true;
    }

    return false;
}

std::string get_file_content(const std::string& _name, const std::string& _id)
{
    std::string fileName = (_id != "" ? (_name + "_" + _id) : _name) + ".txt";
    std::fs::path filePath = get_home_path() / ".cxxm" / "files" / fileName;

    if (std::fs::exists(filePath))
    {
        return file_read_all_text(filePath);
    }
    else
    {
        return defaults::map[_name];
    }
}

std::vector<std::fs::path> get_all_files(
    const std::fs::path& _directory,
    const std::vector<std::string>& _extensions)
{
    std::vector<std::fs::path> filePaths;

    for (const auto& entry : std::fs::recursive_directory_iterator(_directory))
    {
        if (!entry.is_directory())
        {
            const std::string& extension = entry.path().extension().string();

            if (_extensions.size() == 0
                || contains_string(_extensions, extension))
            {
                filePaths.push_back(entry.path());
            }
        }
    }

    return filePaths;
}