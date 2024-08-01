#pragma once
#include <unordered_map>
#include <string>
#include <functional>
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
	std::unordered_map<char, int> calculateFrequencies(const std::string& text);
	std::priority_queue<HuffmanNode, std::vector<HuffmanNode>, CompareHuffmanNode>
		buildHuffmanPriorityQueue(const std::unordered_map<char, int>& frequencies);
	HuffmanNode* buildHuffmanTree(std::priority_queue<HuffmanNode, std::vector<HuffmanNode>,
		CompareHuffmanNode> pq);
	std::unordered_map<char, std::string> buildHuffmanCodes(HuffmanNode* huffmanTree);
	std::string encodeText(const std::unordered_map<char, std::string>&codes, const std::string & text);
	std::unordered_map<char, int> getFrequenciesMap(const std::string& text); //return frequencies from the contents
	std::string decodeText(const std::unordered_map<char, std::string>& codesMap, std::vector<char> text);
	std::unordered_map<std::string, char> swapKeysAndValues(std::unordered_map<char, std::string> originalMap);

public:
	static std::string compress(std::unordered_map<char, std::string>& codes, const std::vector<char>& text);//call to all functions to compress file.
	static std::string decompress(const std::string& text);//call to all functions to decompress file

};

