
#include "project.hpp"

#include "tools.hpp"

#include <fstream>

project::cxxclass::cxxclass(const std::string& _name)
{
    className = std::filesystem::path(_name).filename().string();

    headerCppRelativePath = _name + ".hpp";
    sourceCppRelativePath = _name + ".cpp";

    headerRelativePath = "includes" / headerCppRelativePath;
    sourceRelativePath = "sources" / sourceCppRelativePath;
}

project::project(const std::filesystem::path& _directory)
{
    dirPath = _directory;
    cmakeListsPath = directory() / "CMakeLists.txt";
    isValid = std::filesystem::exists(cmakeListsPath);

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

std::filesystem::path project::directory() const
{
    return dirPath;
}

bool project::add_class(const cxxclass& _class)
{
    size_t pos = cxxmPart.find(')');

    if (pos != std::string::npos)
    {
        cxxmPart.insert(pos, "\n" + to_unix_path(_class.sourceRelativePath).string());

        return true;
    }

    return false;
}

bool project::remove_class(const cxxclass& _class)
{
    const std::string& source = to_unix_path(_class.sourceRelativePath).string();
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

        return true;
    }

    return false;
}

bool project::create_header(const cxxclass& _class)
{
    std::ofstream header(directory() / _class.headerRelativePath);

    if (header.is_open())
    {
        const std::string& headerDefinitionName = get_header_definition_name(_class);

        header << "#ifndef " << headerDefinitionName << "\n";
        header << "#define " << headerDefinitionName << "\n";
        header << "\n";
        header << "class " << _class.className << "\n";
        header << "{\n";
        header << "    \n";
        header << "};\n";
        header << "\n";
        header << "#endif // !" << headerDefinitionName << "";

        header.close();

        return true;
    }

    return false;
}

bool project::create_source(const cxxclass& _class)
{
    std::ofstream source(directory() / _class.sourceRelativePath);

    if (source.is_open())
    {
        source << "\n";
        source << "#include \"" << _class.headerCppRelativePath.string() << "\"\n";
        source << "\n";

        source.close();

        return true;
    }

    return false;
}

bool project::delete_header(const cxxclass& _class)
{
    const std::filesystem::path& headerPath = directory() / _class.headerRelativePath;

    if (std::filesystem::exists(headerPath))
    {
        std::filesystem::remove(headerPath);

        return true;
    }

    return false;
}

bool project::delete_source(const cxxclass& _class)
{
    const std::filesystem::path& sourcePath = directory() / _class.sourceRelativePath;

    if (std::filesystem::exists(sourcePath))
    {
        std::filesystem::remove(sourcePath);

        return true;
    }

    return false;
}

project project::create_new(const std::string& _name)
{
    std::filesystem::create_directory("includes");
    std::filesystem::create_directory("sources");

    std::filesystem::path currentPath = std::filesystem::current_path();

    create_main_source_file(currentPath / "sources");
    create_cmake_lists_file(currentPath, _name);

    return project(currentPath);
}

std::filesystem::path project::find_directory_in_hierarchy()
{
    std::filesystem::path path = std::filesystem::current_path() / "start";

    do
    {
        path = path.parent_path();

        if (std::filesystem::exists(path / "CMakeLists.txt"))
        {
            return path;
        }

    } while (path.has_filename());

    return path;
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

        // TODO check why weird char at the end
        endPart.resize(endPart.length() - 1);

        file.close();

        return true;
    }

    return false;
}

std::string project::get_header_definition_name(const cxxclass& _class)
{
    std::string s = _class.headerCppRelativePath.string();

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

void project::create_cmake_lists_file(
    const std::filesystem::path& _path, const std::string& _projectName)
{
    std::ofstream file(_path / "CMakeLists.txt");

    if (file.is_open())
    {
        file << "cmake_minimum_required(VERSION 3.1)\n";
        file << "\n";
        file << "project(" << _projectName << ")\n";
        file << "\n";
        file << "set(CMAKE_CXX_STANDARD 11)\n";
        file << "\n";
        file << "set(CXXM_SOURCES\n";
        file << "#CXXM_BEGIN\n";
        file << "sources/main.cpp)\n";
        file << "#CXXM_BEGIN\n";
        file << "\n";
        file << "include_directories(includes)\n";
        file << "add_executable(" << _projectName << " ${CXXM_SOURCES})";

        file.close();
    }
}

void project::create_main_source_file(const std::filesystem::path& _path)
{
    std::ofstream source(_path / "main.cpp");

    if (source.is_open())
    {
        source << "\n";
        source << "#include <iostream>\n";
        source << "\n";
        source << "int main(int _argc, char* _argv[])\n";
        source << "{\n";
        source << "    // I love C++\n";
        source << "}";

        source.close();
    }
}