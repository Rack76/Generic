#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <fstream>

namespace Gen
{
	class Parser
	{
	public:
		static void getModelData(const char* filepath, std::vector<float>& positions, std::vector<float>& texCoords);
		static std::string readFile(const char* filepath);

	private:
		static int getNextLineStreamPos(std::fstream& file);
		static void extractLineContent(std::fstream& file, std::string& content, const char* linePattern, const char* firstDelim, const char* lastDelim);
		static void ignoreLinesUntil(const char* pattern, std::fstream& file);
		static void orderArray(std::vector<float>& array, std::vector<int>& indexArray, int stride);
		static void clearIndexArray(std::vector<int>& indexArray);
	};
}

#endif