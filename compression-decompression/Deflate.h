#pragma once
#include <string>
#include <string.h>
class Deflate
{
public:
	static std::string compress(const std::string& text);
	static std::string decompress(const std::string& text);
};

