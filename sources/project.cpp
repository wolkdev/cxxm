
#include "project.hpp"

#include <fstream>

project::project(const std::fs::path& _cmakeListsPath)
{
    cmakeListsPath = _cmakeListsPath;
    dirPath = _cmakeListsPath.parent_path();
    isValid = std::fs::exists(cmakeListsPath);

    if (valid())
    {
        parse_cmake_lists();
    }
}

void project::save()
{
    std::ofstream file(cmakeListsPath);

    if (file.is_open())
    {
        file << startPart;
        file << "#CXXM_BEGIN\n";
        file << cxxmPart;
        file << "#CXXM_END\n";
        file << endPart;

        file.close();
    }
}

bool project::valid() const
{
    return isValid;
}

std::fs::path project::directory() const
{
    return dirPath;
}

std::fs::path project::local_to_project_path(const std::fs::path& _path) const
{
    const std::fs::path& current = std::fs::current_path();
    const std::fs::path& includes = directory() / "includes";
    const std::fs::path& sources = directory() / "sources";

    if (path_contains_base(current, includes))
    {
        return get_path_diff(current, includes) / _path;
    }
    else if (path_contains_base(current, sources))
    {
        return get_path_diff(current, sources) / _path;
    }
    else
    {
        throw std::exception(
            "Invalide Local Path : "
            "You should be in the "
            "includes or sources folder !");
    }
}

bool project::have_class(const cxxclass& _class)
{
    std::string source = to_unix_path(_class.sourceProjectPath).string();
    return cxxmPart.find(source) != std::string::npos;
}

bool project::add_class(const cxxclass& _class)
{
    size_t pos = cxxmPart.find(')');

    if (pos != std::string::npos)
    {
        cxxmPart.insert(pos, "\n" + to_unix_path(_class.sourceProjectPath).string());

        std::cout << "\"" << _class.className << "\"" << " class added" << std::endl;

        return true;
    }
    else
    {
        std::cout << "cannot add class !" << std::endl;
        return false;
    }
}

bool project::remove_class(const cxxclass& _class)
{
    const std::string& source = to_unix_path(_class.sourceProjectPath).string();
    size_t pos = cxxmPart.find(source);

    if (pos != std::string::npos)
    {
        size_t lenght = source.length();

        if (cxxmPart[pos + lenght] == ')')
        {
            if (pos > 0)
            {
                cxxmPart.replace(pos - 1, lenght + 1, "");
            }
            else
            {
                cxxmPart.replace(pos, lenght, "");
            }
        }
        else
        {
            cxxmPart.replace(pos, lenght + 1, "");
        }

        std::cout << "\"" << _class.className << "\"" << " class removed" << std::endl;

        return true;
    }
    else
    {
        std::cout << "cannot remove class !" << std::endl;
        return false;
    }
}

bool project::rename_class(const cxxclass& _from, const cxxclass& _to)
{
    const std::string& sourceFrom = to_unix_path(_from.sourceProjectPath).string();
    const std::string& sourceTo = to_unix_path(_to.sourceProjectPath).string();
    size_t pos = cxxmPart.find(sourceFrom);

    if (pos != std::string::npos)
    {
        cxxmPart.replace(pos, sourceFrom.length(), sourceTo);

        std::cout << "\"" << _from.className << "\"" << " renamed to "
                  << "\"" << _to.className << "\"" << std::endl;

        return true;
    }
    else
    {
        std::cout << "cannot rename class !" << std::endl;
        return false;
    }
}

bool project::parse_cmake_lists()
{
    std::ifstream file(cmakeListsPath);

    startPart.clear();
    cxxmPart.clear();
    endPart.clear();

    if (file.is_open())
    {
        bool begined = false;
        bool ended = false;

        while (!file.eof())
        {
            if (!begined)
            {
                startPart += file.get();

                if (peek_word(file) == "#CXXM_BEGIN")
                {
                    skip_word(file); // skip "#CXXM_BEGIN"
                    file.get(); // skip new line character

                    begined = true;
                }
            }
            else if (!ended)
            {
                cxxmPart += file.get();

                if (peek_word(file) == "#CXXM_END")
                {
                    skip_word(file); // skip "#CXXM_END"
                    file.get(); // skip new line character

                    ended = true;
                }
            }
            else
            {
                endPart += file.get();
            }
        }

        // pop eof char
        
        if (!begined)
        {
            startPart.pop_back();
        }
        else if (!ended)
        {
            cxxmPart.pop_back();
        }
        else
        {
            endPart.pop_back();
        }

        file.close();

        return true;
    }

    return false;
}