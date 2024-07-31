#include "LZ77.h"

//this is fun- helper function  LZ77::decompress to find  from vector <char> a buffer charcter = | 
std::string findIndex(std::vector<char> vec, int& start) {
	auto it = std::find_if(vec.begin() + start, vec.end(), [](char c) {
		return c == '|';
		});
	if (it == vec.end())
		std::cout<<"We need do it---ErrorHandle::NO_BUFFER_CHARACTER_FOUND";
	int help = start;
	int resIndex = it - vec.begin();
	std::string strRes = std::string(vec.begin() + help, vec.begin() + resIndex);
	start = start + strRes.length() + 1;
	return strRes;

}

//int main() {
//	std::vector<char> text = { '0','|','0','|','A','|','0','|','0','|','B','|','2','|','2','|','l' };
//	std::vector<char> res = LZ77::decompress(text);
//	for (int i = 0; i < res.size(); i++) {
//		std::cout << res[i];
//	}
//}

std::vector<char> LZ77::decompress(const std::vector<char> text)
{

	std::vector<char> decompressText;
	for (int i = 0; i < text.size()-1; i ++)
	{
		int offsetText, lengthText;

	    offsetText = std::stoi(findIndex(text, i));
		lengthText = std::stoi(findIndex(text, i));

		char nextChar = text[i++];
		int start = decompressText.size() - offsetText;

		for (int copyText = start; copyText < lengthText + start; copyText++) {
			decompressText.push_back(decompressText[copyText]);
		}
		decompressText.push_back(nextChar);
	
	}
	return decompressText;
}







