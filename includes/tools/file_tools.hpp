#ifndef TOOLS__FILE_TOOLS__HPP
#define TOOLS__FILE_TOOLS__HPP

#include <string>
#include <filesystem>
#include <vector>

namespace std { namespace fs = filesystem; }

std::fs::path get_home_path();

std::fs::path to_unix_path(const std::fs::path& _path);
std::fs::path to_win_path(const std::fs::path& _path);

bool path_contains_base(const std::fs::path& _path, const std::fs::path& _base);
std::fs::path get_path_diff(const std::fs::path& _path, const std::fs::path& _base);

bool find_file_in_hierarchy(const std::string& _fileName, std::fs::path& _path);

std::string file_read_all_text(const std::fs::path& _filePath);
bool file_write_all_text(const std::fs::path& _filePath, const std::string& _text);

void clear_empty_directories(const std::fs::path& _path);

bool create_file(const std::fs::path& _path, const std::string& _text);
bool remove_file(const std::fs::path& _path);
bool move_file(const std::fs::path& _from, const std::fs::path& _to);

std::string get_file_content(const std::string& _name, const std::string& _id = "");

std::vector<std::fs::path> get_all_files(
    const std::fs::path& _directory,
    const std::vector<std::string>& _extensions = { });

#endif // !TOOLS__FILE_TOOLS__HPP