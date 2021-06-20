#include <string>
#include <iostream>
#include <vector>

class Lexer{
public:
    std::string current_text;

    void loadText(std::string* text);
    std::vector<std::string> analyse();
    std::string convertHexToInt(std::string str);
};