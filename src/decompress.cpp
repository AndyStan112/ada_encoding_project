#include "decompress.h"
#include "huffman.h"
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <iostream>
#include <cstring>

void decompressFile(const std::string& inputFile, const std::string& outputFile) {
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }
    
    char magic[2];
    in.read(magic, 2);
    if (std::strncmp(magic, "HF", 2) != 0) {
        std::cerr << "Invalid file format (missing HF magic number)." << std::endl;
        in.close();
        return;
    }
    
    uint32_t uniqueSymbols;
    in.read(reinterpret_cast<char*>(&uniqueSymbols), sizeof(uniqueSymbols));
    
    std::unordered_map<char, uint64_t> frequency;
    for (uint32_t i = 0; i < uniqueSymbols; i++) {
        char c = in.get();
        uint64_t freq;
        in.read(reinterpret_cast<char*>(&freq), sizeof(freq));
        frequency[c] = freq;
    }
    
    Node* root = buildHuffmanTree(frequency);
    
    uint64_t totalBits;
    in.read(reinterpret_cast<char*>(&totalBits), sizeof(totalBits));
    
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        freeTree(root);
        in.close();
        return;
    }
    
    Node* current = root;
    uint64_t bitsRead = 0;
    char byte;
    while (bitsRead < totalBits && in.get(byte)) {
        for (int bitPos = 7; bitPos >= 0 && bitsRead < totalBits; bitPos--) {
            bool bit = (byte >> bitPos) & 1;
            current = bit ? current->right : current->left;
            
            if (!current->left && !current->right) {
                out.put(current->ch);
                current = root;
            }
            bitsRead++;
        }
    }
    
    out.close();
    in.close();
    freeTree(root);
    std::cout << "Decompression complete. Total bits read: " << bitsRead << std::endl;
}
