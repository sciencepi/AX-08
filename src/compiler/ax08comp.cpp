#include "FileLoader.h"
#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv){
    FileLoader fl;
    Lexer lexer;
    Parser parser;

    if (argc > 1){
        std::string loaded_text = fl.loadFile(new std::string(argv[1]));
        lexer.loadText(&loaded_text); // load the text into memory such that the lexer can later process it.
        std::vector<std::string> tokens = lexer.analyse();

        std::vector<int> bytes = parser.parseToBytes(tokens, lexer);

        for (int i = 0; i < bytes.size(); i++){
            std::cout << "0x" << std::hex << bytes[i] << "  ";
        }
        std::cout << "\n";
    }
    return 0;
}