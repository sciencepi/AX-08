#include "Parser.h"
#include "Lexer.h"

#include <vector>
#include <string>
#include <iostream>

std::vector<int> Parser::parseToBytes(std::vector<std::string> tokens, Lexer lex){
    std::vector<int> output;

    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i] == "LOAD_ACCUMULATOR_HIGH"){
            output.push_back(0x1B);
            output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_LOW"){
            output.push_back(0x0B);
            output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
    }
    return output;
}