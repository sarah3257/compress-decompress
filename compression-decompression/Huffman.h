#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <queue>
#include <iostream>

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
	static std::unordered_map<char, int> calculateFrequencies(const std::vector<char>& text);
	static std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>buildHuffmanPriorityQueue(const std::unordered_map<char, int>& freqMap);
	static HuffmanNode* buildHuffmanTree(std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>& pq);
	static std::unordered_map<char, std::string> getHuffmanCodes(HuffmanNode* huffmanTree);
	static void buildCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& codesMap);
	static std::string encodeText(const std::unordered_map<char, std::string>& codes, const std::vector<char>& text);
	static std::unordered_map<std::string, char> swapKeysAndValues(std::unordered_map<char, std::string> originalMap);
public:
	static std::vector<char> compress(std::vector<char>& text, std::unordered_map<char, std::string>& codes);
	//call to all functions to compress file.
	static std::vector<char> decompress(std::vector<char>& text,  std::unordered_map<char, std::string>& codesMap);
	//call to all functions to decompress file.
	using DecompressFunction = std::vector<char>(*)(std::vector<char>&, std::unordered_map<char, std::string>&);

};

