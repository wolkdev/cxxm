
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "project.hpp"

TEST_CASE("Project Search", "[project]")
{
    std::filesystem::path path = project::find_directory_in_hierarchy();
    project p(path);

    REQUIRE(p.valid());
}