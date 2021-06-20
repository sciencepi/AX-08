#include "Lexer.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>

/*
    - Lexer specific functions
    - IE functions that don't belong in the lexer class but are still required for the Lexer to
      function correctly.
*/

void ADD_TOKEN(std::vector<std::string>& tokens, std::string& toks, std::string token){
    tokens.push_back(token);
    toks = "";
}

std::string slice(int start, int stop, std::string str){
    std::string ret = "";
    for (int i = start; i < stop; i++){
        ret += str[i];
    }
    return ret;
}

/*
    - Actual Lexer callfuncs
    - Required for the Lexer to function but are also under the lexer subsidary and therefore
      are elements of the lexer class.
*/

std::string Lexer::convertHexToInt(std::string str){
    unsigned int x;
    std::stringstream ss;
    ss << std::hex << slice(0, str.length()-1, str);
    ss >> x;
    return std::to_string(x);
}

void Lexer::loadText(std::string* text){
    Lexer::current_text = *text;
    std::cout << "Loaded text into memory successfully." << std::endl;
}

std::vector<std::string> Lexer::analyse(){
    std::vector<std::string> tokens;
    std::string tok;

    int line = 0;
    int idx = 0;

    while (idx < Lexer::current_text.length()){
        if (Lexer::current_text[idx] != '\n' && Lexer::current_text[idx] != ' ') tok += Lexer::current_text[idx];
        if (Lexer::current_text[idx] == '\n') line++;

        // instructions
        if (tok == "ldl"){
            ADD_TOKEN(tokens, tok, "LOAD_ACCUMULATOR_LOW");
        }

        // separators
        if (tok == ","){
            ADD_TOKEN(tokens, tok, "COMMA");
        }

        // registers
        if (tok == "%AH"){
            ADD_TOKEN(tokens, tok, "REGISTER_ACCHI");
        }
        if (tok == "%AL"){
            ADD_TOKEN(tokens, tok, "REGISTER_ACCLOW");
        }

        // number CTRL
        if (tok[tok.length()-1] == 'h'){
            ADD_TOKEN(tokens, tok, "INT:"+convertHexToInt(tok));
        }
        ++idx;
    }

    return tokens;
}