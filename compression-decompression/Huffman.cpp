#include "Huffman.h"




std::unordered_map<std::string, char> swapKeysAndValues( std::unordered_map<char, std::string> originalMap) {
    std::unordered_map<std::string, char> swappedMap;
    for (const auto& pair : originalMap) {
        swappedMap[pair.second] = pair.first;
    }
    return swappedMap;
}



//int main()
//{    std::vector<char> text = { '1','0','1','1','0','1','1','0','0' };
//    std::unordered_map<char, std::string> codesMap;
//    codesMap['A'] = "10";codesMap['B'] = "11"; codesMap['C'] = "0";  
//    std::string res = decodeText(codesMap, text);}
std::string Huffman::decodeText(const std::unordered_map<char, std::string>& codesMap, std::vector<char> text)
{
    std::unordered_map<std::string, char> codesMapRev = swapKeysAndValues(codesMap);
    int strJul = 0; 
    std::string keyToFind = "", strResult="";
  
    for (int i = 0; i < text.size(); i++) {
        keyToFind += text[i];
        auto it = codesMapRev.find(keyToFind);
        if (it != codesMapRev.end()) {
            strResult += it->second;
            keyToFind = "";
        }
    }
    if (!keyToFind.empty()) {
        //Error
    }
    return strResult;
}
