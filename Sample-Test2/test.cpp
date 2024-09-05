#include "pch.h"
#include "LZ77.h"
#include "Huffman.h"
#include "Logger.h"
#include <gtest/gtest.h>
#include "Deflate.h"


TEST(LZ77Tests, GetTokensEmptyInput) {
	std::vector<char> input = {};
	auto tokens = LZ77::getTokens(input);
	EXPECT_TRUE(tokens.empty());
}

TEST(LZ77Tests, GetTokensSingleCharacter) {
	std::vector<char> input = { 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
}

TEST(LZ77Tests, GetTokensTwoDifferentCharacters) {
	std::vector<char> input = { 'A', 'B' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(0, 0, 'B'));
}

TEST(LZ77Tests, GetTokensRepeatedCharacters) {
	std::vector<char> input = { 'A', 'A', 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(1, 1, 'A'));
}

TEST(LZ77Tests, GetTokensComplexInput) {
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


TEST(LZ77Test, CompressSimpleInput) {

	// a simple string
	std::vector<char> inputText = { 'a', 'b', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd' };

	// empty unordered_map<char, std::string>
	std::unordered_map<char, std::string> codes;

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
	std::unordered_map<char, std::string> codes;
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
	std::vector<char> text={0};
	std::unordered_map<char, std::string> codes;
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
    
    std::vector<char> text ={'S','T','Z','I','P'};
    std::unordered_map<char, std::string> codes;
    std::unordered_map<char, std::string> codesRes={{'P',"00"},{'I',"01"},{'T',"10"},{'S',"110"},{'Z',"111"}};

    std::vector<char> vectorExpected = Huffman::compress(text, codes);
    std::vector<char> vectorResult = { -41,'@',4 };

   //Checks the text result
    EXPECT_EQ(vectorExpected.size(), vectorResult.size());
    for (int i = 0; i < vectorExpected.size(); ++i) {
        EXPECT_EQ(vectorExpected[i], vectorResult[i]);
    }
    //Checks the codes result
    for (auto item : codes) {
        std::string s = codesRes[item.first];
        std::string s2 = item.second;
            EXPECT_EQ(codesRes[item.first], item.second);          
    }

}
//Checks the  compress empty text
TEST(HuffmanTest, compressEmptyText) {

	std::vector<char> text = { 0};
	std::unordered_map<char, std::string> codes;

	std::vector<char> vectorExpected = Huffman::compress(text, codes);
	std::vector<char> vectorResult={8};
	std::unordered_map<char, std::string> codesRes{{0,"/0"}};

	//Checks the text result
	EXPECT_EQ(vectorExpected.size(), vectorResult.size());
	for (int i = 0; i < vectorExpected.size(); ++i) {
		EXPECT_EQ(vectorExpected[i], vectorResult[i]);
	}


}

//Checks the  decompress huffman 
TEST(HuffmanTest, decompress) {
    
    std::vector<char> text = { '1','1','0','1','0','1','1','1','0','1','0','0' };
    std::unordered_map<char, std::string> codes = { {'P',"00"},{'I',"01"},{'T',"10"},{'S',"110"},{'Z',"111"} };

    std::vector<char> vectorExpected = Huffman::decompress(text, codes);
    std::vector<char> vectorResult = { 'S','T','Z','I','P' } ;

    //Checks the text result
    EXPECT_EQ(vectorExpected.size(), vectorResult.size());
    for (int i = 0; i < vectorExpected.size(); ++i) {
        EXPECT_EQ(vectorExpected[i], vectorResult[i]);
    }
  

}

int main(int argc, char** argv) {

    Logger logger("log.txt");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}




