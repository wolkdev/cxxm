
#include "cxxclass.hpp"

#include "project.hpp"

#include "tools/parsing_tools.hpp"

cxxclass::cxxclass(const project& _project, const std::string& _name)
{
    className = std::fs::path(_name).filename().string();

    projectDir = _project.directory();

    headerRelativePath = _name + ".hpp";
    sourceRelativePath = _name + ".cpp";

    headerProjectPath = "includes" / headerRelativePath;
    sourceProjectPath = "sources" / sourceRelativePath;

    variables.push_back(variable("${CLASS_NAME}", className));
    variables.push_back(variable("${DEFINITION_NAME}", definition_name()));
    variables.push_back(variable("${INCLUDE_PATH}", headerRelativePath.string()));
}

bool cxxclass::create_header()
{
    const std::string& definitionName = definition_name();
    const std::string& text = replace_all(
        get_file_content("header"), variables);
    
    if (create_file(projectDir / headerProjectPath, text))
    {
        std::cout << "header created : " << headerProjectPath << std::endl;
        return true;
    }
    else
    {
        std::cout << "header creation failed : " << headerProjectPath << std::endl;
        return false;
    }
}

bool cxxclass::create_source()
{
    const std::string& text = replace_all(
        get_file_content("source"), variables);
    
    if (create_file(projectDir / sourceProjectPath, text))
    {
        std::cout << "source created : " << sourceProjectPath << std::endl;
        return true;
    }
    else
    {
        std::cout << "source creation failed : " << sourceProjectPath << std::endl;
        return false;
    }
}

bool cxxclass::remove_header()
{
    if (remove_file(projectDir / headerProjectPath))
    {
        std::cout << "header removed : " << headerProjectPath << std::endl;
        return true;
    }
    else
    {
        std::cout << "header removing failed : " << headerProjectPath << std::endl;
        return false;
    }
}

bool cxxclass::remove_source()
{
    if (remove_file(projectDir / sourceProjectPath))
    {
        std::cout << "source removed : " << sourceProjectPath << std::endl;
        return true;
    }
    else
    {
        std::cout << "source removing failed : " << sourceProjectPath << std::endl;
        return false;
    }
}

bool cxxclass::move_header(const cxxclass& _other)
{
    const std::fs::path from = projectDir / headerProjectPath;
    const std::fs::path to = projectDir / _other.headerProjectPath;

    if (move_file(from, to))
    {
        std::string& text = file_read_all_text(to);

        if (replace_all(text,
            definition_name(),
            _other.definition_name()))
        {
            file_write_all_text(to, text);
        }

        std::cout << "header moved from : " << headerProjectPath
                  << " to : " << _other.headerProjectPath << std::endl;

        return true;
    }
    else
    {
        std::cout << "header move failed from : " << headerProjectPath
                  << " to : " << _other.headerProjectPath << std::endl;

        return false;
    }
}

bool cxxclass::move_source(const cxxclass& _other)
{
    const std::fs::path from = projectDir / sourceProjectPath;
    const std::fs::path to = projectDir / _other.sourceProjectPath;

    if (move_file(from, to))
    {
        std::string& text = file_read_all_text(to);

        if (replace_all(text,
            headerRelativePath.string(),
            _other.headerRelativePath.string()))
        {
            file_write_all_text(to, text);            
        }

        std::cout << "source moved from : " << sourceProjectPath
                  << " to : " << _other.sourceProjectPath << std::endl;

        return true;
    }
    else
    {
        std::cout << "source move failed from : " << sourceProjectPath
                  << " to : " << _other.sourceProjectPath << std::endl;

        return false;
    }
}

void cxxclass::replace_all_includes(const cxxclass& _other)
{
    const auto& headers = get_all_files(projectDir / "includes");
    const auto& sources = get_all_files(projectDir / "sources");

    for (const auto& header : headers)
    {
        std::string& text = file_read_all_text(header);

        if (replace_all(text,
            headerRelativePath.string(),
            _other.headerRelativePath.string()))
        {
            if (file_write_all_text(header, text))
            {
                std::cout << "include replaced in file : "
                          << get_path_diff(header, projectDir)
                          << std::endl;
            }
        }
    }

    for (const auto& source : sources)
    {
        std::string& text = file_read_all_text(source);

        if (replace_all(text,
            headerRelativePath.string(),
            _other.headerRelativePath.string()))
        {
            if (file_write_all_text(source, text))
            {
                std::cout << "include replaced in file : "
                          << get_path_diff(source, projectDir)
                          << std::endl;
            }
        }
    }
}

std::string cxxclass::definition_name() const
{
    std::string s = headerRelativePath.string();

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