#include "pch.h"
#include "LZ77.h"
<<<<<<< HEAD

TEST(LZ77Tests, EmptyInput) {
	std::vector<char> input = {};
	auto tokens = LZ77::getTokens(input);
	EXPECT_TRUE(tokens.empty());
}

TEST(LZ77Tests, SingleCharacter) {
	std::vector<char> input = { 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
}

TEST(LZ77Tests, TwoDifferentCharacters) {
	std::vector<char> input = { 'A', 'B' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(0, 0, 'B'));
}

TEST(LZ77Tests, RepeatedCharacters) {
	std::vector<char> input = { 'A', 'A', 'A' };
	auto tokens = LZ77::getTokens(input);
	EXPECT_EQ(tokens.size(), 2);
	EXPECT_EQ(tokens[0], LZ77Token(0, 0, 'A'));
	EXPECT_EQ(tokens[1], LZ77Token(1, 1, 'A'));
}

TEST(LZ77Tests, ComplexInput) {
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
=======
#include "Huffman.h"
#include "Logger.h"
#include <gtest/gtest.h>

TEST(LZ77Test, CompressSimpleInput) {

    // a simple string
    std::vector<char> inputText = {'a', 'b', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd'};
    
    // empty unordered_map<char, std::string>
    std::unordered_map<char, std::string> codes;

    // LZ77compress
    std::vector<char> compressedOutput = LZ77::compress(inputText, codes);

    // the expected output
    std::vector<char> expectedOutput =
    {'0', '|', '0', '|', 'a', '|',
     '0', '|', '0', '|', 'b', '|',
     '2', '|', '2', '|', 'c', '|',
     '0', '|', '0', '|', 'd', '|',
     '4', '|', '3', '|', 'd', '|',
     0}; 
     
    // compare the outputs
    EXPECT_EQ(compressedOutput, expectedOutput);
}

TEST(testsmallinput, TestName) {
	std::vector<char>vec = { '0','1' };
	std::unordered_map<char, int> expected = { {'0',1},
		{'1',1} };
	std::unordered_map<char, int> res = Huffman::calculateFrequencies(vec);
	EXPECT_EQ(expected.size(), res.size());
  EXPECT_TRUE(true);
}

int main(int argc, char** argv) {

    Logger logger("log.txt");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}









>>>>>>> 97c4c2d8da622f1444975c57c0d2f08696f748e5
