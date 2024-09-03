#include "pch.h"
#include "Huffman.h"
#include <gtest/gtest.h>
#include <Logger.h>

TEST(huffmanTest, calculateFrequencies) {
	std::vector<char>vec = { '0','1' };
	std::unordered_map<char, int> expected = { {'0',1},
		{'1',1} };
	std::unordered_map<char, int> res = Huffman::calculateFrequencies(vec);
	EXPECT_EQ(expected.size(), res.size());
  EXPECT_TRUE(true);
}

//Checks the  compress huffman 
TEST(HuffmanTest, compress) {
    Logger logger("log.txt");
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

//Checks the  decompress huffman 
TEST(HuffmanTest, decompress) {
    Logger logger("log.txt");
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
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
