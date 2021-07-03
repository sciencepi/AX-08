#include "Lexer.h"

#include <vector>
#include <iostream>

#ifndef PARSER_H
#define PARSER_H

class Parser{
public:
    unsigned short int org_address = 0x0000;
    std::vector<int> parseToBytes(std::vector<std::string> tokens, Lexer lex);
};

#endif