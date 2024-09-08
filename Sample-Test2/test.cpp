#include "pch.h"
#include "LZ77.h"
#include "Huffman.h"
#include "Logger.h"
#include "BitString.h"
#include <gtest/gtest.h>
#include <BitString.h>
#include "Deflate.h"


bool treeTraversal(HuffmanNode* root1, HuffmanNode* root2) {
	if (root1 == nullptr && root2 == nullptr)
		return true;
	if (root1 == nullptr || root2 == nullptr)
		return false;
	if (root1->c != root2->c)
		return false;
	return treeTraversal(root1->left, root2->left) && treeTraversal(root1->right, root2->right);
}

TEST(LZ77, GetTokensEmptyInput) {
	std::vector<char> input = {};
	auto tokens = LZ77::getTokens(input);
	EXPECT_TRUE(tokens.empty());
}

TEST(LZ77, GetTokensSingleCharacter) {
	std::vector<char> input = { 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
}

TEST(LZ77, GetTokensTwoDifferentCharacters) {
	std::vector<char> input = { 'A', 'B' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(0, 0, 'B'));
}

TEST(LZ77, GetTokensRepeatedCharacters) {
	std::vector<char> input = { 'A', 'A', 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(1, 1, 'A'));
}

TEST(LZ77, GetTokensComplexInput) {
	std::vector<char> input = { 'A', 'B', 'A', 'A', 'C', 'A', 'B', 'A' };
	auto tokens = LZ77::getTokens(input);

	std::vector<LZ77Token> expectedTokens = {
		{0, 0, 'A'},
		{0, 0, 'B'},
		{2, 1, 'A'},
		{0, 0, 'C'},
		{5, 2, 'A'}
	};

	EXPECT_EQ(tokens.size(), expectedTokens.size());
	for (size_t i = 0; i < tokens.size(); ++i) {
		EXPECT_EQ(tokens[i], expectedTokens[i]);
	}
}

TEST(LZ77, Compress) {

	// a simple string
	std::vector<char> inputText = { 'a', 'b', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd' };

	// empty unordered_map<char, std::string>
	std::unordered_map<char, BitString> codes;

	// LZ77compress
	std::vector<char> compressedOutput = LZ77::compress(inputText, codes);

	// the expected output
	std::vector<char> expectedOutput =
	{ '0', '|', '0', '|', 'a', '|',
	 '0', '|', '0', '|', 'b', '|',
	 '2', '|', '2', '|', 'c', '|',
	 '0', '|', '0', '|', 'd', '|',
	 '4', '|', '3', '|', 'd', '|',
	 0 };

	// compare the outputs
	EXPECT_EQ(compressedOutput, expectedOutput);
}

TEST(LZ77, Decompress) {

	// a simple vector<char>
	std::vector<char> compressedText =
	{ '0', '|', '0', '|', 'a', '|',
	 '0', '|', '0', '|', 'b', '|',
	 '2', '|', '2', '|', 'c', '|',
	 '0', '|', '0', '|', 'd', '|',
	 '4', '|', '3', '|', 'd', '|',
	 0 };

	// empty unordered_map<char, std::string>
	std::unordered_map<char, BitString> codesMap;

	// LZ77decompress
	std::vector<char> decompressedOutput = LZ77::decompress(compressedText, codesMap);

	// the expected output
	std::vector<char> expectedOutput = { 'a', 'b', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd' };

	// compare the outputs
	EXPECT_EQ(decompressedOutput, expectedOutput);
}

TEST(huffmanTest, calculateFrequencies) {
	std::vector<char>vec = { '0','1' };
	std::unordered_map<char, int> expected = { {'0',1},
		{'1',1} };
	std::unordered_map<char, int> res = Huffman::calculateFrequencies(vec);
	EXPECT_EQ(expected.size(), res.size());
	EXPECT_TRUE(true);
}



//Checks the  compress deflate with ordinary modifier
TEST(Deflate, compress) {
	std::vector<char> text = { 'S','T','Z', 'S','T','Z', 'S','T','Z' };
	std::unordered_map<char, BitString> codes;
	std::vector<char> vectorExpected = Deflate::compress(text, codes);
	std::vector<char> vectorResult = { -109, 82, 97, 39, -36, -26, -78, -13, -32, 4 };

	////Checks the text result
	EXPECT_EQ(vectorExpected.size(), vectorResult.size());
	for (int i = 0; i < vectorExpected.size(); ++i) {
		EXPECT_EQ(vectorExpected[i], vectorResult[i]);
	}
}
//Checks the  compress empty txt
TEST(Deflate, getEnptyText) {
	std::vector<char> text = { 0 };
	std::unordered_map<char, BitString> codes;
	std::vector<char> vectorExpected = Deflate::compress(text, codes);
	std::vector<char> vectorResult = { -38,0,7 };

	////Checks the text result
	EXPECT_EQ(vectorExpected.size(), vectorResult.size());
	for (int i = 0; i < vectorExpected.size(); ++i) {
		EXPECT_EQ(vectorExpected[i], vectorResult[i]);
	}
}



//Checks the  compress huffman 
TEST(HuffmanTest, compress) {

	std::vector<char> text = { 'S','T','Z','I','P' };
	std::unordered_map<char, BitString> codes;
	std::unordered_map<char, BitString> codesRes = { {'P',BitString("00")},{'I',BitString("01")},{'T',BitString("10")},{'S',BitString("110")},{'Z',BitString("111")} };

	std::vector<char> vectorExpected = Huffman::compress(text, codes);
	std::vector<char> vectorResult = { -41,'@',4 };

	//Checks the text result
	EXPECT_EQ(vectorExpected.size(), vectorResult.size());
	for (int i = 0; i < vectorExpected.size(); ++i) {
		EXPECT_EQ(vectorExpected[i], vectorResult[i]);
	}
	//Checks the codes result
	for (auto item : codes) {
		BitString s = codesRes[item.first];
		BitString s2 = item.second;
		EXPECT_EQ(codesRes[item.first], item.second);
	}

}

//Checks the  decompress huffman 
TEST(HuffmanTest, decompress) {

	std::vector<char> text = { '1','1','0','1','0','1','1','1','0','1','0','0' };
	std::unordered_map<char, BitString> codes = { {'P',BitString("00")},{'I',BitString("01")},{'T',BitString("10")},{'S',BitString("110")},{'Z',BitString("111")} };

	std::vector<char> vectorExpected = Huffman::decompress(text, codes);
	std::vector<char> vectorResult = { 'S','T','Z','I','P' };

	//Checks the text result
	EXPECT_EQ(vectorExpected.size(), vectorResult.size());
	for (int i = 0; i < vectorExpected.size(); ++i) {
		EXPECT_EQ(vectorExpected[i], vectorResult[i]);
	}
}

TEST(HuffmanTest, buildTreeFromMap) {

	std::unordered_map<BitString, char>codes = { {BitString("1011"), 'B'}, { BitString("11"),'G' }, { BitString("00"),'D' }, {BitString("10100"),'V'},{BitString("10101"),'Y'},{BitString("100"),'L'},{BitString("01"),'N'} };
	HuffmanNode* h13 = new HuffmanNode('Y', 0);
	HuffmanNode* h12 = new HuffmanNode('V', 0);
	HuffmanNode* h11 = new HuffmanNode('B', 0);
	HuffmanNode* h10 = new HuffmanNode('$', 0, h12, h13);
	HuffmanNode* h9 = new HuffmanNode('$', 0, h10, h11);
	HuffmanNode* h8 = new HuffmanNode('L', 0);
	HuffmanNode* h7 = new HuffmanNode('G', 0);
	HuffmanNode* h6 = new HuffmanNode('$', 0, h8, h9);
	HuffmanNode* h5 = new HuffmanNode('N', 0);
	HuffmanNode* h4 = new HuffmanNode('D', 0);
	HuffmanNode* h3 = new HuffmanNode('$', 0, h6, h7);
	HuffmanNode* h2 = new HuffmanNode('$', 0, h4, h5);
	HuffmanNode* h1 = new HuffmanNode('$', 0, h2, h3);

	HuffmanNode* res = Huffman::buildHuffmanTreeFromMap(codes);
	bool ans = treeTraversal(res, h1);
	EXPECT_TRUE(ans);
}


int main(int argc, char** argv) {

	Logger logger("log.txt");
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
