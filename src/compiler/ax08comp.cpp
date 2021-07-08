#include "FileLoader.h"
#include "Lexer.h"
#include "Parser.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <stdlib.h>


#define ROM_SIZE_LIMIT 32768

int main(int argc, char** argv){
    FileLoader fl;
    Lexer lexer;
    Parser parser;

    if (argc > 2){
        std::string loaded_text = fl.loadFile(new std::string(argv[1]));
        std::string output_file = std::string(argv[2]);
        lexer.loadText(&loaded_text); // load the text into memory such that the lexer can later process it.
        std::vector<std::string> tokens = lexer.analyse();

        std::vector<int> bytes = parser.parseToBytes(tokens, lexer);
        if (bytes.size() == 2){
            int numz = 0;
            for (int i = 0; i < bytes.size(); i++){
                if (bytes[i] == 0x00){
                    ++numz;
                }
            }
            if (numz == bytes.size()){
                return -1;
            }
        }

        for (int i = 0; i < bytes.size(); i++){
            std::cout << "0x" << std::hex << bytes[i] << "  ";
        }
        std::cout << "\n";

        // output binary to a file.
        if (bytes.size()+parser.org_address > (unsigned)ROM_SIZE_LIMIT){
            std::cout << "Rom image too large to write to file. [FATAL - STOP].\n";
            exit(-1);
        }

        std::cout << std::dec << "ROM Image size: " << bytes.size() << "/" << ROM_SIZE_LIMIT << "\nROM Origin: " << std::hex\
         << parser.org_address << std::endl;
        char rom[ROM_SIZE_LIMIT];
        std::cout << "Creating rom image [1/2]: Creating blank image...\n";
        for (int i = 0; i < ROM_SIZE_LIMIT; i++){
            rom[i] = 0x00;
        }

        std::cout << "Creating rom file [2/2]: Writing bytes...\n";
        // generate binary output
        for (int i = 0; i < bytes.size(); i++){
            rom[i+parser.org_address] = bytes[i];
        }

        // finally, write the output to a file.
        std::ofstream file;
        file.open(output_file, std::ios::out | std::ios::binary | std::ios::trunc);
        file.write(rom, ROM_SIZE_LIMIT);
        file.close();
    }
    return 0;
}