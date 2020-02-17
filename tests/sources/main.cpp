
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "project.hpp"

#include "tools/file_tools.hpp"

TEST_CASE("Project Search", "[project]")
{
    std::fs::path path;
    REQUIRE(find_file_in_hierarchy("CMakeLists.txt", path));
}