#include "Parser.h"
#include "Lexer.h"
#include "Instruction.h"


#include <vector>
#include <string>
#include <iostream>

std::vector<int> ERROR_VECTOR_GENERATOR(){
    std::vector<int> o(0,2);
    return o;
}

std::vector<int> Parser::parseToBytes(std::vector<std::string> tokens, Lexer lex){
    std::vector<int> output;
    int current_line = 0;

    for (int i = 0; i < tokens.size(); i++){
        if (tokens[i] == "NEWLINE"){
            current_line++;
            continue; //  continue is here to increase efficiency as we don't need to check the other statements.
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_HIGH"){
            for (int i = 0; i < tokens.size(); i++)std::cout << tokens[i] << "\n";
            try{
                if (tokens.size()-2 == i) throw 0;
                // parsing code.
                if (lex.slice(0, 4, tokens[i+1]) == "INT:") {
                    output.push_back(I_LDL);
                    output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
                    continue;
                }
                if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    output.push_back(I_LDL+0x10);
                    output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1])) << 8));
                    continue;
                }
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
            //output.push_back(I_LDH+0x10);
            //output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_LOW"){
            output.push_back(I_LDL);
            output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
    }
    return output;
} 