
#include "catch.hpp"

#include "tools/file_tools.hpp"

TEST_CASE("to_unix_path", "[file_tools.hpp]")
{
    REQUIRE(to_unix_path("path\\to\\file") == "path/to/file");
}

TEST_CASE("to_win_path", "[file_tools.hpp]")
{
    REQUIRE(to_unix_path("path/to/file") == "path\\to\\file");
}

TEST_CASE("path_contains_base", "[file_tools.hpp]")
{
    REQUIRE(path_contains_base("absolute/path/to/file", "absolute/path"));
}

TEST_CASE("get_path_diff", "[file_tools.hpp]")
{
    std::fs::path diff = get_path_diff(
        "absolute/path/to/file", "absolute/path");

    REQUIRE(to_unix_path(diff) == "to/file/");
}