#include "huffman.h"
#include <queue>
#include <algorithm>
#include <vector>

Node::Node(char character, uint64_t frequency) : ch(character), freq(frequency), left(nullptr), right(nullptr) {}

bool NodeCompare::operator()(Node* left, Node* right) {
    if (left->freq == right->freq)
        return left->ch > right->ch;
    return left->freq > right->freq;
}
Node* buildHuffmanTree(const std::unordered_map<char, uint64_t>& frequency) {
    std::vector<std::pair<char, uint64_t>> freqVec(frequency.begin(), frequency.end());
    
    std::sort(freqVec.begin(), freqVec.end(), [](const auto &a, const auto &b) {
        return (a.second == b.second) ? (a.first < b.first) : (a.second < b.second);
    });
    
    std::priority_queue<Node*, std::vector<Node*>, NodeCompare> minHeap;
    for (auto &pair : freqVec) {
         minHeap.push(new Node(pair.first, pair.second));
    }
    
    while (minHeap.size() > 1) {
        Node* left = minHeap.top();
        minHeap.pop();
        Node* right = minHeap.top();
        minHeap.pop();
        
        Node* internal = new Node('\0', left->freq + right->freq);
        internal->left = left;
        internal->right = right;
        minHeap.push(internal);
    }
    return minHeap.top();
}

void generateHuffmanCodes(Node* root, const std::string& code, std::unordered_map<char, std::string>& huffmanCodes) {
    if (!root)
        return;
    if (!root->left && !root->right) {
        huffmanCodes[root->ch] = (code.empty() ? "0" : code);
    }
    generateHuffmanCodes(root->left, code + "0", huffmanCodes);
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}
