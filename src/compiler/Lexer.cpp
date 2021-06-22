#include "Lexer.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

/*
    - Lexer specific functions
    - IE functions that don't belong in the lexer class but are still required for the Lexer to
      function correctly.
*/

void ADD_TOKEN(std::vector<std::string>& tokens, std::string& toks, std::string token){
    tokens.push_back(token);
    toks = "";
}


std::string capitalizeString(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c){ return std::toupper(c); });
    return s;
}

/*
    - Actual Lexer callfuncs
    - Required for the Lexer to function but are also under the lexer subsidary and therefore
      are elements of the lexer class.
*/


std::string Lexer::slice(int start, int stop, std::string str){
    std::string ret = "";
    for (int i = start; i < stop; i++){
        ret += str[i];
    }
    return ret;
}

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

    Lexer::current_text = capitalizeString(Lexer::current_text);

    while (idx < Lexer::current_text.length()){
        if (Lexer::current_text[idx] != '\n' && Lexer::current_text[idx] != ' ') tok += Lexer::current_text[idx];
        if (Lexer::current_text[idx] == '\n'){
            line++;
            ADD_TOKEN(tokens, tok, "NEWLINE");
        }

        // instructions
        if (tok == "LDL") ADD_TOKEN(tokens, tok, "LOAD_ACCUMULATOR_LOW");
        if (tok == "LDH") ADD_TOKEN(tokens, tok, "LOAD_ACCUMULATOR_HIGH");
        if (tok == "ADD") ADD_TOKEN(tokens, tok, "ADD");
        if (tok == "ADC") ADD_TOKEN(tokens, tok, "ADD_WITH_CARRY");
        if (tok == "AND") ADD_TOKEN(tokens, tok, "AND");
        if (tok == "OR") ADD_TOKEN(tokens, tok, "OR");
        if (tok == "NOR") ADD_TOKEN(tokens, tok, "NOR");
        if (tok == "CMP") ADD_TOKEN(tokens, tok, "CMP");
        if (tok == "PUSH") ADD_TOKEN(tokens, tok, "PUSH");
        if (tok == "POP") ADD_TOKEN(tokens, tok, "POP");
        if (tok == "LR") ADD_TOKEN(tokens, tok, "LOADREG");
        if (tok == "SR") ADD_TOKEN(tokens, tok, "STOREREG");
        if (tok == "MR") ADD_TOKEN(tokens, tok, "MOVEREG");
        if (tok == "JNZ") ADD_TOKEN(tokens, tok, "JUMPNOTZERO");
        if (tok == "NOP") ADD_TOKEN(tokens, tok, "NOOPERATION");
        if (tok == "HALT") ADD_TOKEN(tokens, tok, "HALT");
        if (tok == "SUB") ADD_TOKEN(tokens, tok, "SUBTRACT");

        // separators
        if (tok == ",") ADD_TOKEN(tokens, tok, "COMMA");
        if (tok == "[") ADD_TOKEN(tokens, tok, "RIGHTBRACKET");
        if (tok == "]") ADD_TOKEN(tokens, tok, "LEFTBRACKET");

        // registers
        if (tok == "%AH") ADD_TOKEN(tokens, tok, "REGISTER_ACCHI");
        if (tok == "%AL") ADD_TOKEN(tokens, tok, "REGISTER_ACCLOW");
        if (tok == "%RBX") ADD_TOKEN(tokens, tok, "REGISTER_1");
        if (tok == "%RAX") ADD_TOKEN(tokens, tok, "REGISTER_0");
        if (tok == "%RCX") ADD_TOKEN(tokens, tok, "REGISTER_2");
        if (tok == "%RDX") ADD_TOKEN(tokens, tok, "REGISTER_3");
        if (tok == "%RZX") ADD_TOKEN(tokens, tok, "REGISTER_4");

        // number CTRL
        if (tok[tok.length()-1] == 'H') ADD_TOKEN(tokens, tok, "INT:"+convertHexToInt(tok));
        ++idx;
    }

    return tokens;
}