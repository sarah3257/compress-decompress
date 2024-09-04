#pragma once
#include <vector>
#include <string>
#include <iostream>

class BitString {
	std::vector<uint8_t> bits;
	size_t length; // Number of actual bits

public:

	BitString();

	// Constructor to initialize with a string of bits (e.g., "101010")
	BitString(const std::string& bitString);

	//
	std::string toString() const;

	// Operator overloading
	bool operator[](size_t index) const;

	// Append another BitString
	BitString& operator+=(const BitString& other);

	// Length of the bit string
	size_t size() const;

	// Convert to a string of characters based on the stored bits
	std::vector<char> toCharVector() const;

	// Copy constructor
	BitString(const BitString& other);

	// Copy assignment operator
	BitString& operator=(const BitString& other);

	// Equality operator
	bool operator==(const BitString& other) const;
};
// Hash function for BitString
namespace std {
	template <>
	struct hash<BitString> {
		size_t operator()(const BitString& bitString) const {
			return hash<std::string>()(string(bitString.toCharVector().begin(), bitString.toCharVector().end())); // hash by converting to string
		}
	};
	template <>
	struct equal_to<BitString> {
		bool operator()(const BitString& lhs, const BitString& rhs) const {
			return lhs == rhs; // Use the BitString equality operator
		}
	};
}