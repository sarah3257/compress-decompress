#include "pch.h"
#include "Huffman.h"
TEST(testsmallinput, TestName) {
	std::vector<char>vec = { '0','1' };
	std::unordered_map<char, int> expected = { {'0',1},
		{'1',1} };
	std::unordered_map<char, int> res = Huffman::calculateFrequencies(vec);
	EXPECT_EQ(expected.size()+1, res.size());
  EXPECT_TRUE(true);
}

//#include <gtest/gtest.h>
//
//// דוגמה לבדיקה פשוטה
//TEST(SimpleTest, Addition) {
//    EXPECT_EQ(2 + 2, 4);
//}
//
//// פונקציה ראשית להפעלת כל הבדיקות
//int main(int argc, char **argv) {
//    ::testing::InitGoogleTest(&argc, argv);
//    return RUN_ALL_TESTS();
//}
//
