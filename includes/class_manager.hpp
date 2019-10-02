#ifndef CLASS_MANAGER__HPP
#define CLASS_MANAGER__HPP

#include "cmakelists_manager.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string get_header_definition_name(
    const std::filesystem::path& _headerRelativePath)
{
    std::string s = _headerRelativePath.string();

    char c;
    bool lastLowerCase = false;
    bool currentUpperCase;

    for (size_t i = 0; i < s.length(); i++)
    {
        c = s[i];

        currentUpperCase = c >= 65 && c <= 92;

        if (c == '/' || c == '\\')
        {
            s[i] = '_';
            s.insert(i, "_");
        }
        else if (c == '.')
        {
            s[i] = '_';
            s.insert(i, "_");
        }
        else if (currentUpperCase && lastLowerCase)
        {
            s.insert(i, "_");
        }

        s[i] = toupper(s[i]);

        lastLowerCase =  c >= 97 && c <= 122;
    }

    return s;
}

void create_header_file(
    const std::filesystem::path& _headerPath,
    const std::string& _className)
{
    std::ofstream header(_headerPath);

    if (header.is_open())
    {
        // TODO : format headerDefinition
        std::string headerDefinition = _className + "__" + _headerPath.extension().string();

        header << "#ifndef " << headerDefinition << "\n";
        header << "#define " << headerDefinition << "\n";
        header << "\n";
        header << "class " << _className << "\n";
        header << "{\n";
        header << "    \n";
        header << "};\n";
        header << "\n";
        header << "#endif // !" << headerDefinition << "";

        header.close();
    }
}

void create_source_file(
    const std::filesystem::path& _headerRelativePath,
    const std::filesystem::path& _sourcePath,
    const std::string& _className)
{
    std::ofstream source(_sourcePath);

    if (source.is_open())
    {
        source << "\n";
        source << "#include \"" << _headerRelativePath.string() << "\"\n";
        source << "\n";

        source.close();
    }
}

bool add_source_to_cxxm_list(std::string& _list, const std::filesystem::path& _source)
{
    size_t pos = _list.find(')');

    if (pos != std::string::npos)
    {
        _list.insert(pos, "\n" + _source.string());

        return true;
    }
    
    return false;
}

bool remove_source_to_cxxm_list(std::string& _list, const std::filesystem::path& _source)
{
    const std::string& source = _source.string();
    size_t pos = _list.find(source);

    if (pos != std::string::npos)
    {
        size_t end = source.length() - 1;

        if (_list[end + 1] == ')')
        {
            _list.replace(pos, end, "");
        }
        else
        {
            _list.replace(pos, end + 1, "");
        }

        return true;
    }

    return false;
}

void add_class(const std::filesystem::path& _classRelativePath)
{
    const std::filesystem::path& cmakepath = get_cmakelists_path();

    if (cmakepath.has_filename()) // cmake file found
    {
        const std::filesystem::path projectDir = cmakepath.parent_path();

        const std::string& className = _classRelativePath.filename().string();
        const std::filesystem::path& headerRelativePath = "includes/" + _classRelativePath.string() + ".hpp";
        const std::filesystem::path& sourceRelativePath = "sources/" + _classRelativePath.string() + ".cpp";

        create_header_file(projectDir / headerRelativePath, className);
        create_source_file(headerRelativePath, projectDir / sourceRelativePath, className);

        std::string startPart;
        std::string cxxmPart;
        std::string endPart;

        get_cmake_parts(cmakepath, startPart, cxxmPart, endPart);

        if (add_source_to_cxxm_list(cxxmPart, sourceRelativePath))
        {
            save_cmake_file(cmakepath, startPart, cxxmPart, endPart);
        }
        else
        {
            // TODO : write it failed
        }
    }
}

void remove_class(const std::filesystem::path& _classRelativePath)
{
    const std::filesystem::path& cmakepath = get_cmakelists_path();

    if (cmakepath.has_filename()) // cmake file found
    {
        const std::filesystem::path projectDir = cmakepath.parent_path();

        const std::filesystem::path& headerRelativePath = "includes/" + _classRelativePath.string() + ".hpp";
        const std::filesystem::path& sourceRelativePath = "sources/" + _classRelativePath.string() + ".cpp";

        if (std::filesystem::exists(projectDir / headerRelativePath))
        {
            std::filesystem::remove(projectDir / headerRelativePath);
        }
        else
        {
            // TODO : write it failed
        }

        if (std::filesystem::exists(projectDir / sourceRelativePath))
        {
            std::filesystem::remove(projectDir / sourceRelativePath);
        }
        else
        {
            // TODO : write it failed
        }

        std::string startPart;
        std::string cxxmPart;
        std::string endPart;

        get_cmake_parts(cmakepath, startPart, cxxmPart, endPart);

        if (remove_source_to_cxxm_list(cxxmPart, sourceRelativePath))
        {
            save_cmake_file(cmakepath, startPart, cxxmPart, endPart);
        }
        else
        {
            // TODO : write it failed
        }
    }
}

#endif // !CLASS_MANAGER__HPP