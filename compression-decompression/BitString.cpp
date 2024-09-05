#include "BitString.h"

#define BIT_MASK(pos) (1 << (7 - (pos)))


BitString::BitString() : length(0) {}

// Constructor to initialize with a string of bits (e.g., "101010")
BitString::BitString(const std::string& bitString) {
	length = bitString.size();
	bits.resize((length + 7) / 8, 0); // Allocate enough bytes to store the bits
	for (size_t i = 0; i < length; ++i) {
		if (bitString[i] == '1') {
			//bits[i / 8] |= (1 << (7 - (i % 8)));
			bits[i / 8] |= BIT_MASK(i % 8);
		}
	}
}


// Operator overloading
bool BitString::operator[](size_t index) const {
	if (index >= length) throw std::out_of_range("Index out of range");
//	return (bits[index / 8] >> (7 - (index % 8))) & 1;
	return (bits[index / 8] & BIT_MASK(index % 8)) != 0;
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

// �����: ������ ������� ����� ����� ������ ������
 // Direct access to bits vector for advanced operations

const std::vector<uint8_t>& BitString::getBits()
{
	return bits;
}
// Length of the bit string
size_t BitString::size() const {
	return length;
}

// Convert to a string of characters based on the stored bits
std::vector<char> BitString::toCharVector() const {
	std::vector<char> result;
	result.reserve(length / 8);
	for (size_t i = 0; i < length / 8 || (i == length / 8) && (length % 8); i++) {
		uint8_t byte = bits[i];
		result.push_back(static_cast<char>(byte));
	}
	//add the added bits
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

// Equality operator
bool BitString::operator==(const BitString& other) const {
	if (length != other.length)
		return false; // Different lengths
	return bits == other.bits;
}