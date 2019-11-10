#ifndef PROJECT__HPP
#define PROJECT__HPP

#include <filesystem>
#include <string>

class project
{
    public:
    struct cxxclass
    {
        std::string className;
        std::filesystem::path headerRelativePath;
        std::filesystem::path sourceRelativePath;
        std::filesystem::path headerProjectPath;
        std::filesystem::path sourceProjectPath;

        cxxclass(const std::string& _name);
    };

    private:
    std::filesystem::path dirPath;
    std::filesystem::path cmakeListsPath;

    bool isValid;

    std::string startPart;
    std::string cxxmPart;
    std::string endPart;

    public:
    project(const std::filesystem::path& _directory);
    void save();

    bool valid() const;
    std::filesystem::path directory() const;
    std::filesystem::path local_to_project_path(const std::filesystem::path& _path) const;

    bool add_class(const cxxclass& _class);
    bool rename_class(const cxxclass& _from, const cxxclass& _to);
    bool remove_class(const cxxclass& _class);

    bool create_header(const cxxclass& _class);
    bool create_source(const cxxclass& _class);

    bool move_header(const cxxclass& _from, const cxxclass& _to);
    bool move_source(const cxxclass& _from, const cxxclass& _to);

    bool delete_header(const cxxclass& _class);
    bool delete_source(const cxxclass& _class);

    static project create_new(const std::string& _name);
    static std::filesystem::path find_directory_in_hierarchy();

    private:
    bool parse_cmake_lists();

    std::string get_header_definition_name(const cxxclass& _class);

    static void create_cmake_lists_file(
        const std::filesystem::path& _path,
        const std::string& _projectName);
    
    static void create_main_source_file(const std::filesystem::path& _path);
};

#endif // !PROJECT__HPP