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
    int i = 0;
    while (i+1 <= tokens.size()){
        if (tokens[i] == "NEWLINE"){
            current_line++;
            i += 1;
            continue; //  continue is here to increase efficiency as we don't need to check the other statements.
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_HIGH"){
            for (int i = 0; i < tokens.size(); i++)std::cout << tokens[i] << "\n";
            try{
                if (tokens.size()-2 == i) throw 0;
                // parsing code.
                if (lex.slice(0, 5, tokens[i+1]) == "IMM8:") {
                    output.push_back(I_LDH);
                    output.push_back(stoi(lex.slice(5, tokens[i+1].length(), tokens[i+1])));
                    i+=2;
                }
                else if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    output.push_back(I_LDH+0x10);
                    output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1]))));
                    i+= 2;
                }
                else if (tokens[i+1] == "RIGHTBRACKET" && lex.slice(0, 5, tokens[i+2]) == "IMM8:" && tokens[i+3] == "COMMA" &&\
                 lex.slice(0, 3, tokens[i+4]) == "REG" && tokens[i+5] == "LEFTBRACKET"){

                    output.push_back(I_LDH+0x20);
                    output.push_back(stoi(lex.slice(5, tokens[i+1].length(), tokens[i+2])));
                    output.push_back((stoi(lex.slice(tokens[i+4].length()-1, tokens[i+4].length(), tokens[i+4]))));
                    i += 2;
                }
                else if (lex.slice(0, 6, tokens[i+1]) == "IMM16:"){
                    int integer_value = stoi(lex.slice(6, tokens[i+1].length(), tokens[i+1]));
                    int upper_8       = (integer_value & 0xff00) >> 8;
                    int lower_8       = (integer_value & 0x00ff);
                    output.push_back(I_LDH+0x30);

                    // the system is little endian
                    output.push_back(lower_8);
                    output.push_back(upper_8);

                    i += 2;
                }else{
                    std::cout << "Error - unexpected token on line: " << current_line << "\n";
                }
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
            //output.push_back(I_LDH+0x10);
            //output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_LOW"){
            for (int i = 0; i < tokens.size(); i++)std::cout << tokens[i] << "\n";
            try{
                if (tokens.size()-2 == i) throw 0;
                // parsing code.
                if (lex.slice(0, 5, tokens[i+1]) == "IMM8:") {
                    output.push_back(I_LDL);
                    output.push_back(stoi(lex.slice(5, tokens[i+1].length(), tokens[i+1])));
                    i+=2;
                }
                else if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    output.push_back(I_LDL+0x10);
                    output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1]))));
                    i+= 2;
                }
                else if (tokens[i+1] == "RIGHTBRACKET" && lex.slice(0, 5, tokens[i+2]) == "IMM8:" && tokens[i+3] == "COMMA" &&\
                 lex.slice(0, 3, tokens[i+4]) == "REG" && tokens[i+5] == "LEFTBRACKET"){

                    output.push_back(I_LDL+0x20);
                    output.push_back(stoi(lex.slice(5, tokens[i+1].length(), tokens[i+2])));
                    output.push_back((stoi(lex.slice(tokens[i+4].length()-1, tokens[i+4].length(), tokens[i+4]))));
                    i += 2;
                }
                else if (lex.slice(0, 6, tokens[i+1]) == "IMM16:"){
                    int integer_value = stoi(lex.slice(6, tokens[i+1].length(), tokens[i+1]));
                    int upper_8       = (integer_value & 0xff00) >> 8;
                    int lower_8       = (integer_value & 0x00ff);
                    output.push_back(I_LDL+0x30);

                    // the system is little endian
                    output.push_back(lower_8);
                    output.push_back(upper_8);

                    i += 2;
                }else{
                    std::cout << "Error - unexpected token on line: " << current_line << "\n";
                }
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
            //output.push_back(I_LDH+0x10);
            //output.push_back(stoi(lex.slice(4, tokens[i+1].length(), tokens[i+1])));
        }

        // preprocessor macros

        if (tokens[i] == "PREP_ORG" && lex.slice(0, 6, tokens[i+1]) == "IMM16:"){
            Parser::org_address = stoi(lex.slice(6, tokens[i+1].length(), tokens[i+1]));
            i += 2;
        }
        i++;
    }
    return output;
} 