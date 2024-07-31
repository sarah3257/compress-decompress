#include <vector>
#include <string>
class HandleFile
{
public:
	std::vector<char> readFromFile(const std::string& filePath);
	void writeToFile(const std::string& filePath, const std::vector<char> strContent);
};
