#include <iostream>
#include <string>
#include "compress.h"
#include "decompress.h"

void printUsage(std::string name){
    std::cerr << "Usage: " << name << " -c|-d <input_file> <output_file>" << std::endl;
    exit(1);
}

int main(int argc, char* argv[]) {
    std::string name = argv[0];
    if (argc != 4) {
        printUsage(name);
    }
    
    std::string mode = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    
    if (mode == "-c") {
        compressFile(inputFile, outputFile);
    } else if (mode == "-d") {
        decompressFile(inputFile, outputFile);
    } else {
        printUsage(name);
    }
    
    return 0;
}
