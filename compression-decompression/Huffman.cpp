#include "Huffman.h"
std::unordered_map<char, int> Huffman::calculateFrequencies(const std::vector<char>& text) {
	std::unordered_map<char, int>freqMap;
	for (auto ch : text)
		freqMap[ch]++;
	return freqMap;
}

std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode> Huffman::buildHuffmanPriorityQueue(const std::unordered_map<char, int>& freqMap) {
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode> pq;
	//Transfer the data from the map to the priority queue
	for (auto pair : freqMap)
		pq.push(new HuffmanNode(pair.first, pair.second));
	return pq;
}

HuffmanNode* Huffman::buildHuffmanTree(std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode>& pq) {
	while (pq.size() > 1) {
		//Get the char with min frequency
		HuffmanNode* left = pq.top();
		pq.pop();
		//Get the char with the second min frequency
		HuffmanNode* right = pq.top();
		pq.pop();
		//Create parent with the sum of the frequencies
		HuffmanNode* parent = new HuffmanNode('$', left->freq + right->freq, left, right);
		//Attach this parent to the priority queue
		pq.push(parent);
	}
	HuffmanNode* res = pq.top();
	return res;
}

std::unordered_map<char, std::string> Huffman::getHuffmanCodes(HuffmanNode* huffmanTree) {
	std::unordered_map<char, std::string>codesMap;
	buildCodes(huffmanTree, "", codesMap);
	return codesMap;
}

void Huffman::buildCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& codesMap) {
	if (!root)
		return;
	if (root->c != '$')
		codesMap[root->c] = str;
	buildCodes(root->left, str + '0', codesMap);
	buildCodes(root->right, str + '1', codesMap);
}

std::string Huffman::encodeText(const std::unordered_map<char, std::string>& codes, const std::vector<char>& text) {
	std::string result = "";
	for (auto ch : text)
		result.append(codes.at(ch));
	return result;
}

//std::string str = "ABBBBGFFFFFG";
//str = Huffman::compress(std::vector<char>(str.begin(), str.end()));
//std::cout << str;
std::string Huffman::compress(std::unordered_map<char, std::string>& codes, const std::vector<char>& text) {
	std::unordered_map<char, int> freqMap = calculateFrequencies(text);
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode> pq = buildHuffmanPriorityQueue(freqMap);
	HuffmanNode* tree = buildHuffmanTree(pq);
	codes = getHuffmanCodes(tree);
	std::string result = encodeText(codes, text);
	return result;
}