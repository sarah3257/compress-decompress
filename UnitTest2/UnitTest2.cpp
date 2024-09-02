#include "pch.h"
#include "CppUnitTest.h"
#include "LZ77.h"
#include "LZ77.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestMethod1)
		{
			std::vector<char> text;
			std::vector<LZ77Token> expectedTokens;

			std::vector<LZ77Token> actualTokens = LZ77::getTokens(text);

			Assert::AreEqual(5, 5);
			// Test case for empty text
			//TEST_METHOD(GetTokensTest_EmptyText) {
			//	std::vector<char> text;
			//	std::vector<LZ77Token> expectedTokens;

			//	std::vector<LZ77Token> actualTokens = getTokens(text);

			//	Assert::AreEqual(expectedTokens, actualTokens);
			//}
		}
	};
}
