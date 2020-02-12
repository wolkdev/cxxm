#ifndef TOOLS__PARSING_TOOLS__HPP
#define TOOLS__PARSING_TOOLS__HPP

#include <iostream>
#include <string>

std::string peek_word(std::istream& _istream);
void skip_word(std::istream& _istream);

void replace_all(
    std::string& _string,
    const std::string& _from,
    const std::string& _to);

#endif // !TOOLS__PARSING_TOOLS__HPP