#include "compress.h"
#include "huffman.h"
#include <fstream>
#include <unordered_map>
#include <cstdint>
#include <iostream>

void compressFile(const std::string& inputFile, const std::string& outputFile) {
    std::unordered_map<char, uint64_t> frequency;
    std::ifstream in(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error opening input file: " << inputFile << std::endl;
        return;
    }
    const size_t chunkSize = 4096;
    char buffer[chunkSize];
    while (in.read(buffer, chunkSize) || in.gcount() > 0) {
        std::streamsize bytesRead = in.gcount();
        for (std::streamsize i = 0; i < bytesRead; i++) {
            frequency[buffer[i]]++;
        }
    }
    in.close();
    
    Node* root = buildHuffmanTree(frequency);
    
    std::unordered_map<char, std::string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);
    
    std::ofstream out(outputFile, std::ios::binary);
    if (!out) {
        std::cerr << "Error opening output file: " << outputFile << std::endl;
        freeTree(root);
        return;
    }
    
    out.write("HF", 2);
    
    uint32_t uniqueSymbols = frequency.size();
    out.write(reinterpret_cast<const char*>(&uniqueSymbols), sizeof(uniqueSymbols));
    
    for (auto& pair : frequency) {
        out.put(pair.first);
        uint64_t freq = pair.second;
        out.write(reinterpret_cast<const char*>(&freq), sizeof(freq));
    }
    
    uint64_t totalBits = 0;
    std::streampos totalBitsPos = out.tellp();
    out.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));
    
    in.open(inputFile, std::ios::binary);
    if (!in) {
        std::cerr << "Error reopening input file: " << inputFile << std::endl;
        out.close();
        freeTree(root);
        return;
    }
    
    uint8_t bitBuffer = 0;
    int bitCount = 0;
    totalBits = 0;
    
    while (in.read(buffer, chunkSize) || in.gcount() > 0) {
        std::streamsize bytesRead = in.gcount();
        for (std::streamsize i = 0; i < bytesRead; i++) {
            char c = buffer[i];
            const std::string& code = huffmanCodes[c];
            for (char bitChar : code) {
                bitBuffer = (bitBuffer << 1) | (bitChar - '0');
                bitCount++;
                totalBits++;
                if (bitCount == 8) {
                    out.put(bitBuffer);
                    bitBuffer = 0;
                    bitCount = 0;
                }
            }
        }
    }
    
    if (bitCount > 0) {
        bitBuffer <<= (8 - bitCount);
        out.put(bitBuffer);
    }
    out.flush();
    

    out.seekp(totalBitsPos);
    out.write(reinterpret_cast<const char*>(&totalBits), sizeof(totalBits));
    
    out.close();
    freeTree(root);
    std::cout << "Compression complete. Total encoded bits: " << totalBits << std::endl;
}
