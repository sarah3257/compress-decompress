#include "pch.h"
#include "LZ77.h"
#include "Huffman.h"
#include "Logger.h"
#include <gtest/gtest.h>

TEST(LZ77, Compress) {

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
    std::unordered_map<char, std::string> codesMap;

    // LZ77decompress
    std::vector<char> decompressedOutput = LZ77::decompress(compressedText, codesMap);

    // the expected output
    std::vector<char> expectedOutput = { 'a', 'b', 'a', 'b', 'c', 'd', 'a', 'b', 'c', 'd' };

    // compare the outputs
    EXPECT_EQ(decompressedOutput, expectedOutput);
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
