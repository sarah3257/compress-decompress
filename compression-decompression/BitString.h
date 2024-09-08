#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#define BIT_MASK(pos) (1 << (7 - (pos)))


class BitString {
	std::vector<uint8_t> bits;
	size_t length; // Number of actual bits

public:
	BitString();

	// Constructor to initialize with a string of bits (e.g., "101010")
	BitString(const std::string& bitString);

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
	inline bool operator==(const BitString& other) const {
		return length == other.length && bits == other.bits;
	}

	// Operator overloading
	inline bool operator[](size_t index) const {
		return (bits[index / 8] & BIT_MASK(index % 8));
	}
};
// Hash function for BitString
namespace std {
	template <>
	struct hash<BitString> {
		size_t operator()(const BitString& bitString) const {
			const std::vector<char>& vec = bitString.toCharVector();
			size_t hash = 0;
			for (char c : vec) {
				hash ^= std::hash<char>()(c) + 0x9e3779b9 + (hash << 6) + (hash >> 2); // A simple hash combiner
			}
			return hash;
		}
	};
	template <>
	struct equal_to<BitString> {
		bool operator()(const BitString& lhs, const BitString& rhs) const {
			return lhs == rhs; // Use the BitString equality operator
		}
	};
}