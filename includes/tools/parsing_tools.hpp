#ifndef TOOLS__PARSING_TOOLS__HPP
#define TOOLS__PARSING_TOOLS__HPP

#include <iostream>
#include <vector>
#include <string>

struct variable
{
    std::string name;
    std::string value;
};

std::string peek_word(std::istream& _istream);
void skip_word(std::istream& _istream);

std::string& replace_all(
    std::string& _string,
    const std::string& _from,
    const std::string& _to);

std::string replace_all(
    const std::string& _string,
    const std::vector<variable>& _variables);

#endif // !TOOLS__PARSING_TOOLS__HPP