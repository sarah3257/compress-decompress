#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <queue>
#include <iostream>
#include "BitString.h"

struct HuffmanNode {
	char c;
	int freq;
	HuffmanNode* right, * left;
	HuffmanNode(char c, int freq, HuffmanNode* left = nullptr, HuffmanNode* right = nullptr)
		:c(c), freq(freq), left(left), right(right) {}
};

struct CompareHuffmanNode {
	bool operator()(const HuffmanNode* a, const HuffmanNode* b) {
		return a->freq > b->freq;
	}
};

class Huffman
{
	static std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>buildHuffmanPriorityQueue(const std::unordered_map<char, int>& freqMap);
	static HuffmanNode* buildHuffmanTree(std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>& pq);
	static std::unordered_map<char, BitString> getHuffmanCodes(HuffmanNode* huffmanTree);
	static void buildCodes(HuffmanNode* root, std::string str, std::unordered_map<char, BitString>& codesMap);
	static BitString encodeText(const std::unordered_map<char, BitString>& codes, const std::vector<char>& text);
	static std::unordered_map<BitString, char> swapKeysAndValues(std::unordered_map<char, BitString> originalMap);
public:
	static std::unordered_map<char, int> calculateFrequencies(const std::vector<char>& text);
	static std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, BitString>& codes);
	//call to all functions to compress file.
	static std::vector<char> decompress(std::vector<char>& text,  std::unordered_map<char, BitString>& codesMap);
	//call to all functions to decompress file.
	using DecompressFunction = std::vector<char>(*)(std::vector<char>&, std::unordered_map<char, BitString>&);

};

