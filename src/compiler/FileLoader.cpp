#include "FileLoader.h"

#include <iostream>
#include <string>
#include <fstream>

std::string FileLoader::loadFile(std::string* fileName){
    std::string line;
    std::string out;
    std::ifstream currentFile(*fileName);
    if (currentFile.is_open()){
        while (getline(currentFile, line)){
            out += line + "\n";
        }
        currentFile.close();
    }else{
        throw std::invalid_argument("Recieved invalid file address");
    }

    return out;
}