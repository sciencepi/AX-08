#include "Lexer.h"
#include "FileLoader.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

#include <stdlib.h>

/*
    - Lexer specific functions
    - IE functions that don't belong in the lexer class but are still required for the Lexer to
      function correctly.
*/

typedef struct Macro{
    std::vector<std::string> macro_code;
    std::string macro_name;
}Macro;

int findMacro(std::vector<Macro> macros, std::string name){
    int i;
    for (i = 0; i < macros.size(); i++){
        if (macros[i].macro_name == name) return i;
    }
    return -1;
}

Macro getMacro(std::vector<Macro> macros, std::string name){
    for (int i = 0; i < macros.size(); i++){
        if (macros[i].macro_name == name){
            return macros[i];
        }
    }
    Macro errorMacro;
    errorMacro.macro_name = "";
}

void ADD_TOKEN(std::vector<std::string>& tokens, std::string& toks, std::string token, std::vector<Macro>& macros,\
 bool macroStart, std::string currentMacro, bool macroFinished){
    if (!macroStart){
        tokens.push_back(token);
        toks = "";
    }else{
        
        if (macroFinished){
            int midx = findMacro(macros, currentMacro);
            if (midx == -1){
                std::cout << "MACRO Error - Macro not found:" << currentMacro << std::endl;
                return;
            }
            macros[midx].macro_code.push_back(token);
            toks = "";
        }
    }
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

void LEXER_ERROR(std::string err, int l){
    std::cout << "Error [LINE:" << l << "]: " << err << std::endl;
    exit(-1);
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

// macros

std::vector<std::string> Lexer::analyse(){
    std::vector<std::string> tokens;
    std::string tok;
    FileLoader file;

    int line = 0;
    int idx = 0;

    bool includeStart = false;
    bool macroStart = false;
    bool macroFinished = true;

    bool macroExpressionStart = false;
    std::string macroExpression = "";

    std::string current_MacroName = "";
    Macro currentMacro;

    std::string incFileName = "";

    Lexer::current_text = capitalizeString(Lexer::current_text);
    std::vector<Macro> macros;

    while (idx < Lexer::current_text.length()){
        if (Lexer::current_text[idx] != '\n' && Lexer::current_text[idx] != ' ') tok += Lexer::current_text[idx];
        if (Lexer::current_text[idx] == '\n'){
            line++;
            if (includeStart){
                std::string filecontents = file.loadFile(&incFileName);
                filecontents = "\n" + filecontents;
                filecontents = capitalizeString(filecontents);
                Lexer::current_text.insert(idx, filecontents);
                std::cout << "Loaded include file: " << incFileName << std::endl;
                includeStart = false;
                incFileName = "";
            }

            if (macroStart){
                currentMacro.macro_name = current_MacroName;
                if (!macroFinished)
                    macros.push_back(currentMacro);
                macroFinished = true;
            }
            if (macroExpressionStart){
                macroExpressionStart = false;
                Macro mac = getMacro(macros, macroExpression);
                if (mac.macro_name == ""){
                    LEXER_ERROR("Cannot find macro \"" + macroExpression + "\"", line);
                    exit(-1);
                }
                // paste in the macro
                for (std::string macroE : mac.macro_code){
                    tokens.push_back(macroE);
                }
                macroExpression = "";
            }
            ADD_TOKEN(tokens, tok, "NEWLINE", macros, macroStart, current_MacroName, macroFinished);
        }
        if (tok == "@ENDMACRO"){
            macroStart = false;
            macroFinished = true;
            tok = "";
        }

        // instructions
        if (tok == "LDL") ADD_TOKEN(tokens, tok, "LOAD_ACCUMULATOR_LOW", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "LDH") ADD_TOKEN(tokens, tok, "LOAD_ACCUMULATOR_HIGH", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "ADD") ADD_TOKEN(tokens, tok, "ADD", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "AND") ADD_TOKEN(tokens, tok, "AND", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "OR") ADD_TOKEN(tokens, tok, "OR", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "NOR") ADD_TOKEN(tokens, tok, "NOR", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "CMP") ADD_TOKEN(tokens, tok, "CMP", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "PUSH") ADD_TOKEN(tokens, tok, "PUSH", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "POP") ADD_TOKEN(tokens, tok, "POP", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "LR") ADD_TOKEN(tokens, tok, "LOADREG", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "SR") ADD_TOKEN(tokens, tok, "STOREREG", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "MR") ADD_TOKEN(tokens, tok, "MOVEREG", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "JNZ") ADD_TOKEN(tokens, tok, "JUMPNOTZERO", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "NOP") ADD_TOKEN(tokens, tok, "NOOPERATION", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "STOP") ADD_TOKEN(tokens, tok, "HALT", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "SUB") ADD_TOKEN(tokens, tok, "SUBTRACT", macros, macroStart, current_MacroName, macroFinished);

        // separators
        if (tok == ",") ADD_TOKEN(tokens, tok, "COMMA", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "[") ADD_TOKEN(tokens, tok, "RIGHTBRACKET", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "]") ADD_TOKEN(tokens, tok, "LEFTBRACKET", macros, macroStart, current_MacroName, macroFinished);

        // registers
        if (tok == "%AH") ADD_TOKEN(tokens, tok, "REGISTER_5", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%AL") ADD_TOKEN(tokens, tok, "REGISTER_6", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%RBX") ADD_TOKEN(tokens, tok, "REGISTER_1", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%RAX") ADD_TOKEN(tokens, tok, "REGISTER_0", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%RCX") ADD_TOKEN(tokens, tok, "REGISTER_2", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%RDX") ADD_TOKEN(tokens, tok, "REGISTER_3", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "%RZX") ADD_TOKEN(tokens, tok, "REGISTER_4", macros, macroStart, current_MacroName, macroFinished);

        // include file
        if (includeStart){
            if (tok == "\""){
                tok = "";
            }else{
                incFileName += tok;
                tok = "";
            }
        }

        // preprocessor macros
        
        if (tok == "@ORG") ADD_TOKEN(tokens, tok, "PREP_ORG", macros, macroStart, current_MacroName, macroFinished);
        if (tok == "@INCLUDE"){
            // this is a lexical operator
            includeStart = true;
            tok = "";
        }
        if (macroExpressionStart){
            macroExpression += tok;
            tok = "";
        }
        if (tok == "."){
            macroExpressionStart = true;
            tok = "";
        }
        if (macroStart && !macroFinished){
            if (tok == "\""){
                tok = "";
            }else{
                current_MacroName += tok;
                tok = "";
            }
        }
        if (tok == "@MACRO"){
            current_MacroName = "";
            macroStart = true;
            macroFinished = false;
            tok = "";
        }

        

        // number CTRL
        if (tok[tok.length()-1] == 'H'){ // 2-way: 16-bit numbers and 8-bit numbers
            if (tok.length() == 5){ // 16-bits
                ADD_TOKEN(tokens, tok, "IMM16:"+convertHexToInt(tok), macros, macroStart, current_MacroName, macroFinished);
            }
            else if (tok.length() == 3){
                ADD_TOKEN(tokens, tok, "IMM8:"+convertHexToInt(tok), macros, macroStart, current_MacroName, macroFinished);
            }
            else if (tok.length() == 1){
                // do nothing as this is probably the HL register.
            }
            else{
                std::cout << tok << std::endl;
                LEXER_ERROR("Illegal number type - non 16/8 bit number detected.", line);
            }
        }
        ++idx;
    }
    return tokens;
}