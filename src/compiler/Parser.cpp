#include "Parser.h"
#include "Lexer.h"
#include "Instruction.h"


#include <vector>
#include <string>
#include <iostream>

std::vector<int> Parser::parseToBytes(std::vector<std::string> tokens, Lexer lex){
    std::vector<int> output;

    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i] == "LOAD_ACCUMULATOR_HIGH"){
            output.push_back(I_LDH+0x10);
            output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_LOW"){
            output.push_back(I_LDL);
            output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
    }
    return output;
}