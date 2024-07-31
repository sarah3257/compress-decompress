#pragma once
#include <unordered_map>
#include <string>
#include <functional>
#include <queue>

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
	static HuffmanNode* buildHuffmanTree(std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>&pq);
	static std::unordered_map<char, std::string> getHuffmanCodes(HuffmanNode* huffmanTree);
	static void buildCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& codesMap);
	static std::string encodeText(const std::unordered_map<char, std::string>& codes, const std::vector<char>& text);
	static std::unordered_map<char, int> getFrequenciesMap(const std::string& text);
	static std::string decodeText(const std::unordered_map<char, int>& freqMap);
public:
	static std::string compress(const std::vector<char>& text);//call to all functions to compress file.
	static std::string decompress(const std::string& text);//call to all functions to decompress file

};

