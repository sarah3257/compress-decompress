#include "pch.h"
#include "Huffman.h"
#include <gtest/gtest.h>

TEST(testsmallinput, TestName) {
	std::vector<char>vec = { '0','1' };
	std::unordered_map<char, int> expected = { {'0',1},
		{'1',1} };
	std::unordered_map<char, int> res = Huffman::calculateFrequencies(vec);
	EXPECT_EQ(expected.size(), res.size());
  EXPECT_TRUE(true);
}


// דוגמה לבדיקה פשוטה
TEST(SimpleTest, Addition) {
    EXPECT_EQ(2 + 2, 4);
}

// דוגמה לבדיקה עם תנאי
TEST(SimpleTest, BooleanCondition) {
    EXPECT_TRUE(3 > 2);
}

// דוגמה לבדיקה של אוסף
TEST(SimpleTest, VectorSize) {
    std::vector<int> vec = { 1, 2, 3, 4, 5 };
    EXPECT_EQ(vec.size(), 5);
}

// פונקציה ראשית להפעלת כל הבדיקות
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
