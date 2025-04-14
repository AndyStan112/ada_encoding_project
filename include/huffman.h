#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <cstdint>
#include <unordered_map>
#include <string>

struct Node {
    char ch;
    uint64_t freq;
    Node* left;
    Node* right;
    Node(char character, uint64_t frequency);
};

struct NodeCompare {
    bool operator()(Node* left, Node* right);
};

Node* buildHuffmanTree(const std::unordered_map<char, uint64_t>& frequency);

void generateHuffmanCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes);

void freeTree(Node* root);

#endif
