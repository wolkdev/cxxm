#ifndef TOOLS__PARSING_TOOLS__HPP
#define TOOLS__PARSING_TOOLS__HPP

#include <iostream>
#include <vector>
#include <string>

struct variable
{
    std::string name;
    std::string value;

    variable(const std::string& _name, const std::string& _value) :
        name(_name), value(_value) { }
};

std::string peek_word(std::istream& _istream);
void skip_word(std::istream& _istream);

bool contains_string(
    const std::vector<std::string>& _vector,
    const std::string& _string);

bool replace_all(
    std::string& _string,
    const std::string& _from,
    const std::string& _to);

std::string replace_all(
    const std::string& _string,
    const std::vector<variable>& _variables);

#endif // !TOOLS__PARSING_TOOLS__HPP