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




std::unordered_map<std::string, char> Huffman::swapKeysAndValues( std::unordered_map<char, std::string> originalMap) {
    std::unordered_map<std::string, char> swappedMap;
    for (const auto& pair : originalMap) {
        swappedMap[pair.second] = pair.first;
    }
    return swappedMap;
}



//int main()
//{    std::vector<char> text = { '1','0','1','1','0','1','1','0','0' };
//    std::unordered_map<char, std::string> codesMap;
//    codesMap['A'] = "10";codesMap['B'] = "11"; codesMap['C'] = "0";  
//    std::string res = decodeText(codesMap, text);}
std::string Huffman::decodeText(const std::unordered_map<char, std::string>& codesMap, std::vector<char> text)
{
    std::unordered_map<std::string, char> codesMapRev = swapKeysAndValues(codesMap);
    int strJul = 0; 
    std::string keyToFind = "", strResult="";
  
    for (int i = 0; i < text.size(); i++) {
        keyToFind += text[i];
        auto it = codesMapRev.find(keyToFind);
        if (it != codesMapRev.end()) {
            strResult += it->second;
            keyToFind = "";
        }
    }
    if (!keyToFind.empty()) {
        //Error
    }
    return strResult;
}

std::string Huffman::decompress(const std::string& text) {
	std::string str = "";
	return str;
}
