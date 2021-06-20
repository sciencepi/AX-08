#include <string>
#include <iostream>
#include <vector>

#ifndef LEXER_H
#define LEXER_H

class Lexer{
public:
    std::string current_text;

    void loadText(std::string* text);
    std::vector<std::string> analyse();
    std::string convertHexToInt(std::string str);
    std::string slice(int start, int stop, std::string str);
};

#endif