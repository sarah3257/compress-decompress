#include "Deflate.h"

std::string Deflate::password = "stzip";

std::string Deflate::compressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes) {

	std::vector<char> lz77Compress = LZ77::compress(buffer);
	std::string huffmanCompress = Huffman::compress(codes, lz77Compress);
	return huffmanCompress;
}
void Deflate::compress(const std::string& fileName) {

	HandleFile handleFile(fileName, true);
	handleFile.insertPassword(password);
	//read the buffers
	std::vector<char> buffer;
	std::string compressText;
	int sss = handleFile.getFileSizeMinusCurrentSize();
	while (handleFile.getFileSizeMinusCurrentSize()) {
		buffer = handleFile.readBufferCompress();
		std::unordered_map<char, std::string> codes;
		int sss = handleFile.getFileSizeMinusCurrentSize();
		compressText = compressDeflate(buffer, codes);
		//עד כאן טוב
		handleFile.writeBufferCompress(codes, compressText);
	}

}
void Deflate::decompress(const std::string& text) {

	// check if the password is correct
	if (!isCorrectPassword(text)) {
		std::cout << "The password is invalid";
	}
	else {
		std::vector<char> buffer;
		std::vector<char>  decompressRes;
		HandleFile handleFile(text, false,password.size());

		while (handleFile.getFileSizeMinusCurrentSize()) {
			std::unordered_map<char, std::string> codes;
			buffer = handleFile.readBufferDecompress(codes);
			int sss = handleFile.getFileSizeMinusCurrentSize();
			// we need get a code map
			decompressRes = decompressDeflate(buffer, codes);
			handleFile.writeBufferDecompress(decompressRes);
		}
	}
}

std::vector<char> Deflate::decompressDeflate(const std::vector<char>& buffer, std::unordered_map<char, std::string>& codes)
{
	std::vector<char> decompressHuffman = Huffman::decompress(codes, buffer);
	std::vector<char>decompressLZ77 = LZ77::decompress(decompressHuffman);
	return decompressLZ77;
}

bool Deflate::isCorrectPassword(const std::string& text) {

	// open the file
	std::ifstream sourceFile(text, std::ios::binary);
	if (!sourceFile.is_open()) {
		throw std::runtime_error("Failed to open the file.");
	}
	std::string readPassword(password.size(), '\0');

	// read the password
	sourceFile.read(&readPassword[0], password.size());
	if (sourceFile.gcount() != password.size()) {
		sourceFile.close();
		throw std::runtime_error("Failed to read the password from file.");
	}
	sourceFile.close();
	return readPassword == password;
}