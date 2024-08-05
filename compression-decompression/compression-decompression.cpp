#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include "Deflate.h"

int main() {
    //std::string text = "01000001"; // דוגמה של מחרוזת ביטים
    //std::ofstream outputFile("file.bin", std::ios::binary);

    //std::vector<char> buffer;
    //for (int i = 0; i < text.size(); i += 8) {
    //    std::string byteString = text.substr(i, 8);
    //    std::bitset<8> byte(byteString);
    //    buffer.push_back(static_cast<char>(byte.to_ulong()));
    //}
    //outputFile.write(buffer.data(), buffer.size());
    //outputFile.close();

    //std::ifstream inputFile("file.bin", std::ios::binary);
    //if (!inputFile) {
    //    std::cerr << "Error opening file." << std::endl;
    //    return 1;
    //}

    //inputFile.seekg(0, std::ios::end);
    //std::streampos fileSize = inputFile.tellg();
    //inputFile.seekg(0, std::ios::beg);

    //std::vector<char> res(fileSize);
    //inputFile.read(res.data(), fileSize);
    //inputFile.close();

    //if (res.empty()) {
    //    std::cout << "The file is empty." << std::endl;
    //}
    //else {
    //    std::cout << "Read " << res.size() << " bytes from file." << std::endl;
    //    std::cout << "Contents of res: ";
    //    for (char c : res) {
    //        std::cout << std::bitset<8>(c) << ' ';
    //    }
    //    std::cout << std::endl;
    //}

    //return 0;
    std::string filePath = "inputFile.txt";
    Deflate::compress(filePath);
<<<<<<< Updated upstream
    Deflate::decompress("inputFileSTZ_COMPRESS.bin");
=======
    Deflate::decompress("inputFileSTZ_COMPRESS.txt");

>>>>>>> Stashed changes

}