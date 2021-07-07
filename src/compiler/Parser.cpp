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

void APPEND_ARITHMETIC_OPERATIONS(Lexer* lexer, std::vector<std::string> tokens, int& i, std::vector<int>& output, int line){
    int instruction_to_push = 0xFF;
    
    // cant use switch here because its too annoying to implement
    // so im gonna do the ugly if spam :(
    
    std::string token = tokens[i];
    if (token == "ADD") instruction_to_push = I_ADD;
    if (token == "AND") instruction_to_push = I_AND;
    if (token == "OR") instruction_to_push = I_OR;
    if (token == "NOR") instruction_to_push = I_NOR;
    if (token == "CMP") instruction_to_push = I_CMP;
    if (token == "SUBTRACT") instruction_to_push = I_SUB;

    if (instruction_to_push == 0xFF) return;
    std::cout << instruction_to_push << std::endl;

    try{
        if (lexer->slice(0,3,tokens[i+1]) == "REG"){
            if (tokens[i+2] == "COMMA"){
                if (lexer->slice(0, 6, tokens[i+3]) == "IMM16:"){
                    int integer_value = stoi(lexer->slice(6, tokens[i+3].length(), tokens[i+3]));
                    int upper_8       = (integer_value & 0xff00) >> 8;
                    int lower_8       = (integer_value & 0x00ff);

                    output.push_back(instruction_to_push);
                    output.push_back(lower_8);
                    output.push_back(upper_8);
                    i += 2;
                    return;
                }
                if (lexer->slice(0, 3, tokens[i+3]) == "REG"){
                    output.push_back(instruction_to_push + 0x10);
                    output.push_back((stoi(lexer->slice(tokens[i+3].length()-1, tokens[i+3].length(), tokens[i+3]))));
                    i += 2;
                    return;
                }
            }else{ throw 0; }
        }else{ throw 0; }
    }catch(int e){
        std::cout << "Error on line " << line << std::endl;
    }
}

std::vector<int> Parser::parseToBytes(std::vector<std::string> tokens, Lexer lex){
    std::vector<int> output;
    int current_line = 2;
    int i = 0;
    while (i+1 <= tokens.size()){
        if (tokens[i] == "NEWLINE"){
            current_line++;
            i += 1;
            continue; //  continue is here to increase efficiency as we don't need to check the other statements.
        }
        // for arithmetic functions, a more suitable checker would use SWITCH as all of the arithmetic
        // instruction use the same code.

        APPEND_ARITHMETIC_OPERATIONS(&lex, tokens, i, output, current_line);
        if (tokens[i] == "LOAD_ACCUMULATOR_HIGH"){
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
        if (tokens[i] == "NOOPERATION"){
            output.push_back(I_NOP);
        }
        if (tokens[i] == "HALT"){
            output.push_back(I_HALT);
        }
        if (tokens[i] == "LOAD_ACCUMULATOR_LOW"){
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
        }

        if (tokens[i] == "JUMPNOTZERO"){
            output.push_back(I_JNZ);
        }

        if (tokens[i] == "STOREREG"){
            try{
                // parsing code.
                if (tokens[i+3] == "RIGHTBRACKET" && lex.slice(0, 5, tokens[i+4]) == "IMM8:" && tokens[i+5] == "COMMA" &&\
                 lex.slice(0, 3, tokens[i+6]) == "REG" && tokens[i+7] == "LEFTBRACKET"){

                    output.push_back(I_SR+0x30);
                    output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1]))));
                    output.push_back(stoi(lex.slice(5, tokens[i+3].length(), tokens[i+4])));
                    output.push_back((stoi(lex.slice(tokens[i+6].length()-1, tokens[i+6].length(), tokens[i+6]))));
                    i += 2;
                }
                else if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    if (tokens[i+2] == "COMMA"){
                        if (lex.slice(0, 6, tokens[i+3]) == "IMM16:"){
                            int integer_value = stoi(lex.slice(6, tokens[i+3].length(), tokens[i+3]));
                            int upper_8       = (integer_value & 0xff00) >> 8;
                            int lower_8       = (integer_value & 0x00ff);
                            output.push_back(I_SR+0x10);

                            // the system is little endian
                            output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1]))));
                            output.push_back(lower_8);
                            output.push_back(upper_8);
                        }
                        else if (lex.slice(0, 3, tokens[i+3]) == "REG"){
                            output.push_back(I_SR+0x20);
                            output.push_back(stoi(lex.slice(tokens[i+3].length()-1, tokens[i+3].length(), tokens[i+3])));
                        }
                    }
                    i+= 2;
                }
                
                else{
                    std::cout << "Error - unexpected token on line: " << current_line << "\n";
                }
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
        }

        if (tokens[i] == "LOADREG"){
            try{
                if (tokens.size()-2 == i) throw 0;
                // parsing code.
                if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    if (tokens[i+2] == "COMMA"){
                        if (lex.slice(0, 5, tokens[i+3]) == "IMM8:"){
                            int integer_value = stoi(lex.slice(5, tokens[i+3].length(), tokens[i+3]));
                            output.push_back(I_LR);
                            output.push_back((stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1]))));
                            output.push_back(integer_value);
                        }else{
                            throw 0;
                        }
                    }
                    i+= 2;
                }else{
                    std::cout << "Error - unexpected token on line: " << current_line << "\n";
                }
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
        }

        if (tokens[i] == "MOVEREG"){
            try{
                if (lex.slice(0, 3, tokens[i+1]) == "REG" && tokens[i+2] == "COMMA" && lex.slice(0, 3, tokens[i+3]) == "REG"){
                    output.push_back(I_MR);
                    output.push_back(stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1])));
                    output.push_back(stoi(lex.slice(tokens[i+3].length()-1, tokens[i+3].length(), tokens[i+3])));
                }else{
                    std::cout << "Error on line " << current_line << ".\n";
                    exit(-1);
                }
                i+=2;
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
        }

        if (tokens[i] == "POP"){
            if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                output.push_back(I_POP);
                output.push_back(stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1])));

                i+=2;
            }else{
                std::cout << "Error on line " << current_line << ".\n";
                exit(-1);
            }
        }

        if (tokens[i] == "PUSH"){
            try{
                if (lex.slice(0, 3, tokens[i+1]) == "REG"){
                    output.push_back(I_PUSH);
                    output.push_back(stoi(lex.slice(tokens[i+1].length()-1, tokens[i+1].length(), tokens[i+1])));
                }else{
                    std::cout << "Error on line " << current_line << ".\n";
                    exit(-1);
                }
                i+=2;
            }catch(int e){
                std::cout << "Error on line " << current_line << ".\n";
            }
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