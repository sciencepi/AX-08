#include "FileLoader.h"
#include "Lexer.h"

#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv){
    FileLoader fl;
    Lexer lexer;

    if (argc > 1){
        std::string loaded_text = fl.loadFile(new std::string(argv[1]));
        lexer.loadText(&loaded_text); // load the text into memory such that the lexer can later process it.
        std::vector<std::string> tokens = lexer.analyse();

        for (int i = 0; i < tokens.size(); i++){
            std::cout << tokens[i] << ",\n";
        }
    }
    return 0;
}