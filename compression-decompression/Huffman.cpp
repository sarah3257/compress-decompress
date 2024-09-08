#include "Huffman.h"
#include "Logger.h"
#include <bitset>
#include "BitString.h"

// Counts character frequencies in the text and returns a map.
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

// Constructs the Huffman tree and returns the root node.
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

std::unordered_map<char, BitString> Huffman::getHuffmanCodes(HuffmanNode* huffmanTree) {
	std::unordered_map<char, BitString>codesMap;
	buildCodes(huffmanTree, "", codesMap);
	return codesMap;
}

void Huffman::buildCodes(HuffmanNode* root, std::string str, std::unordered_map<char, BitString>& codesMap) {
	if (!root)
		return;
	if (!root->left)
		codesMap[root->c] = str;
	else {
		buildCodes(root->left, str + '0', codesMap);
		buildCodes(root->right, str + '1', codesMap);
	}
}

// Encodes the text using the provided Huffman codes and returns the encoded string.
BitString Huffman::encodeText(const std::unordered_map<char, BitString>& codes, const std::vector<char>& text) {
	BitString result("");
	int index = 0;
	for (auto ch : text) {
		result += codes.at(ch);
	}
	return result;
}

// Compresses the text using Huffman coding and returns the encoded string.
std::vector<char> Huffman::compress(std::vector<char>& text, std::unordered_map<char, BitString>& codes) {
	Logger::logInfo(Logger::START_FUNCTION + "compress " + Logger::IN_CLASS + "Huffman");
	std::unordered_map<char, int> freqMap = calculateFrequencies(text);
	std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareHuffmanNode> pq = buildHuffmanPriorityQueue(freqMap);
	HuffmanNode* tree = buildHuffmanTree(pq);
	codes = getHuffmanCodes(tree);
	BitString result = encodeText(codes, text);
	std::vector<char> stringResult = result.toCharVector();
	Logger::logInfo(Logger::END_FUNCTION + " compress " + Logger::IN_CLASS + "Huffman");
	return stringResult;
}


//decompression

// Swaps keys and values in the map and returns the new map.
std::unordered_map<BitString, char> Huffman::swapKeysAndValues(const std::unordered_map<char, BitString>& originalMap) {
	std::unordered_map<BitString, char> swappedMap;
	for (const auto& pair : originalMap) {
		swappedMap[pair.second] = pair.first;
	}
	return swappedMap;
}

// Decodes the Huffman-encoded text and returns the original text.
std::vector<char> Huffman::decompress(std::vector<char>& text, std::unordered_map<char, BitString>& codesMap) {
	Logger::logInfo(Logger::START_FUNCTION + "decompress " + Logger::IN_CLASS + "Huffman");
	std::unordered_map<BitString, char> codesMapRev = swapKeysAndValues(codesMap);
	HuffmanNode* head = buildHuffmanTreeFromMap(codesMapRev);
	HuffmanNode* temp = head;
	std::string strResult = "";
	for (int i = 0; i < text.size(); i++) {
		if (text[i] == '0')
			temp = temp->left;
		else
			temp = temp->right;
		if (temp->left == nullptr) {
			strResult += temp->c;
			temp = head;
		}
	}
	std::vector<char> vecRes(strResult.begin(), strResult.end());
	Logger::logInfo(Logger::END_FUNCTION + " decompress " + Logger::IN_CLASS + "Huffman");
	return vecRes;
}

HuffmanNode* Huffman::buildHuffmanTreeFromMap(std::unordered_map<BitString, char>codes) {
	HuffmanNode* head = new HuffmanNode('$', 0);
	HuffmanNode* temp;
	for (auto pair : codes) {
		temp = head;
		for (int i = 0; i < pair.first.size(); i++)
		{
			if (pair.first[i] == 0) {
				if (temp->left == nullptr)
					temp->left = new HuffmanNode('$', 0);
				temp = temp->left;
			}
			else {
				if (temp->right == nullptr)
					temp->right = new HuffmanNode('$', 0);
				temp = temp->right;
			}
		}
		temp->c = pair.second;
	}
	return head;
}
