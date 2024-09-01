#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "Deflate.h"
class Test
{
public:
	static void playTest();
private:
	static void createRegularFile(const std::string& filename);
	static void testEmptyFile();
	static void createEmptyFile(const std::string& filename);
	static void writeRandomValuesToTextFile(const std::string& filename1);
	static void testRandomFile();
	static void testSmallFile();
	static bool areFilesEqual(const std::string& file1, const std::string& file2);
	static void writeSmallFile(const std::string filename1);
	static void writeCharacterToFile(const std::string filename1);
	static void testOneCharacterFile();
	static void createFileWithZeros(const std::string filename1);
	static void testZeroFile();
	static void testRegularFile();
	static void testControl_Z_File();
	static void createControl_Z_File(const std::string& filename1);
	static void testSizeGBFile();
	static void createSizeGBFile(const std::string& filename1);
	static void compressAndDecompress(const std::string& filename);

};



/*
#include "pch.h"
#include "CppUnitTest.h"
#include "../compression-decompression/LZ77.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestGetTokens_EmptyInput)
		{
			std::vector<char> input = {};
			std::vector<LZ77Token> tokens = LZ77::getTokens(input);
			Assert::AreEqual(static_cast<size_t>(0), tokens.size(), L"Expected no tokens for empty input.");
		}

		TEST_METHOD(TestGetTokens_SingleCharacter)
		{
			std::vector<char> input = { 'a' };
			std::vector<LZ77Token> tokens = LZ77::getTokens(input);
			Assert::AreEqual(static_cast<size_t>(1), tokens.size(), L"Expected one token for single character input.");
			Assert::AreEqual(0, tokens[0].offset, L"Expected offset to be 0.");
			Assert::AreEqual(0, tokens[0].length, L"Expected length to be 0.");
			Assert::AreEqual('a', tokens[0].nextChar, L"Expected next char to be 'a'.");
		}

		TEST_METHOD(TestGetTokens_RepeatedCharacters)
		{
			std::vector<char> input = { 'a', 'a', 'a', 'a' };
			std::vector<LZ77Token> tokens = LZ77::getTokens(input);
			Assert::AreEqual(static_cast<size_t>(4), tokens.size(), L"Expected four tokens for repeated characters.");
			Assert::AreEqual(0, tokens[0].offset, L"Expected offset for first token to be 0.");
			Assert::AreEqual(0, tokens[0].length, L"Expected length for first token to be 0.");
			Assert::AreEqual('a', tokens[0].nextChar, L"Expected next char for first token to be 'a'.");
			// Validate other tokens as needed
		}

		TEST_METHOD(TestGetTokens_ExampleInput)
		{
			std::vector<char> input = { 'a', 'b', 'c', 'a', 'b', 'c', 'a' };
			std::vector<LZ77Token> tokens = LZ77::getTokens(input);
			Assert::AreEqual(static_cast<size_t>(7), tokens.size(), L"Expected seven tokens for example input.");
			// Validate specific tokens for expected values
			Assert::AreEqual(0, tokens[0].offset, L"Expected first token offset to be 0.");
			Assert::AreEqual(0, tokens[0].length, L"Expected first token length to be 0.");
			Assert::AreEqual('a', tokens[0].nextChar, L"Expected first token next char to be 'a'.");
			Assert::AreEqual(1, tokens[3].offset, L"Expected fourth token offset to be 1 (repeating 'a').");
			Assert::AreEqual(1, tokens[3].length, L"Expected fourth token length to be 1 (length of 'a').");
			Assert::AreEqual('a', tokens[3].nextChar, L"Expected fourth token next char to be 'a'.");
		}
	};
}


*/

