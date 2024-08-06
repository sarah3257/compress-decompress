﻿#include <iostream>
#include <fstream>
#include <vector>
#include <bitset>
#include "Deflate.h"

int main() {
    //// שלב 1: כתיבת המחרוזת לקובץ בינארי
    //std::string text = "01000001"; // דוגמה של מחרוזת ביטים
    //std::ofstream outputFile("file.bin", std::ios::binary);

    //// וקטור זמני לאחסון המידע המומר
    //std::vector<char> buffer;
    //for (int i = 0; i < text.size(); i += 8) {
    //    std::string byteString = text.substr(i, 8);
    //    std::bitset<8> byte(byteString);
    //    buffer.push_back(static_cast<char>(byte.to_ulong()));
    //}
    //outputFile.write(buffer.data(), buffer.size());
    //outputFile.close();

    //// שלב 2: קריאת הקובץ הבינארי לתוך וקטור של char
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

    //// שלב 3: בדיקת התוצאה בוקטור res
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
    std::string filePath = "large_repeating_text.txt";
    Deflate::compress(filePath);
    std::cout << "finishCompress\n";
    Deflate::decompress("large_repeating_textSTZ_COMPRESS.bin");

}