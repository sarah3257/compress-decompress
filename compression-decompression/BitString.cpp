#include "BitString.h"

BitString::BitString() : length(0) {}

// Constructor to initialize with a string of bits (e.g., "101010")
BitString::BitString(const std::string& bitString) {
	length = bitString.size();
	bits.resize((length + 7) / 8, 0); // Allocate enough bytes to store the bits
	for (size_t i = 0; i < length; ++i)
		if (bitString[i] == '1')
			bits[i / 8] |= BIT_MASK(i % 8);
}

// Append another BitString
BitString& BitString::operator+=(const BitString& other) {
	size_t newLength = length + other.length;
	bits.resize((newLength + 7) / 8, 0);
	for (size_t i = 0; i < other.length; ++i)
		bits[(length + i) / 8] |= (other[i] << (7 - ((length + i) % 8)));
	length = newLength;
	return *this;
}

// Length of the bit string
size_t BitString::size() const {
	return length;
}

// Convert to a string of characters based on the stored bits

std::vector<char> BitString::toCharVector() const {
	std::vector<char> result;
	result.reserve(bits.size() + 1); // Reserve space for bits and addedBits
	result.insert(result.end(), bits.begin(), bits.end());

	int addedBits = (length % 8) ? (8 - (length % 8)) : 0;
	result.push_back(static_cast<char>(addedBits));

	return result;
}


// Copy constructor
BitString::BitString(const BitString& other) : bits(other.bits), length(other.length) {}

// Copy assignment operator
BitString& BitString::operator=(const BitString& other) {
	if (this != &other) { // Self-assignment check
		bits = other.bits; // This should copy the vector correctly
		length = other.length; // Copy the length
	}
	return *this; // Return the current object
}
