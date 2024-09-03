#include "pch.h"
#include "LZ77.h"

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