
#include "catch.hpp"

#include "tools/parsing_tools.hpp"

#include <string>
#include <sstream>

TEST_CASE("peek_word", "[parsing_tools.hpp]")
{
    std::stringstream ss("this is a simple test");

    REQUIRE(peek_word(ss) == "this");
    REQUIRE(ss.tellg() == 0);
}

TEST_CASE("skip_word", "[parsing_tools.hpp]")
{
    std::stringstream ss("this is a simple test");
    skip_word(ss);

    REQUIRE(ss.tellg() == 4);
}

TEST_CASE("replace_all", "[parsing_tools.hpp]")
{
    std::string s = "this is a simple test with a simple word to replace";
    replace_all(s, "simple", "complex");

    REQUIRE(s == "this is a complex test with a complex word to replace");
}

TEST_CASE("replace_all (variables)", "[parsing_tools.hpp]")
{
    std::string s = "this is a simple test with a simple word to replace";

    std::vector<variable> variables =
        { { "simple", "complex" }, { "word", "variable" } };

    const std::string& r = replace_all(s, variables);

    REQUIRE(r == "this is a complex test with a complex variable to replace");
}