#include "GameEngine/Util/Parser.h"
#include "GameEngine/Util/Util.h"
#include <string>
#include <sstream>

namespace Gen
{
	void Parser::getModelData(const char* filepath, std::vector<float>& positions, std::vector<float>& texCoords)
	{
		std::string geometryId;
		std::string positionsString;
		std::string texCoordsString;
		std::string indicesString;
		std::vector<int> positionIndices;
		std::vector<int> textureIndices;

		std::string line;
		std::fstream file(filepath, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
		assertNoAbort(file.is_open(), "could not open file " + std::string(filepath));
		extractLineContent(file, geometryId, "geometry id=\"", "\"", "\"");
		extractLineContent(file, positionsString,  std::string(geometryId + "-positions-array").c_str(), ">", "<");
		
		std::stringstream ss(positionsString);
		float position;
		while (!ss.eof())
		{
			ss >> position;
			positions.push_back(position);
		}

		extractLineContent(file, texCoordsString, std::string(geometryId + "-map-0-array").c_str(), ">", "<");

		ss = std::stringstream(texCoordsString);
		float texCoord;
		while (!ss.eof())
		{
			ss >> texCoord;
			texCoords.push_back(texCoord);
		}

		extractLineContent(file, indicesString, "<p>", ">", "<");

		ss = std::stringstream(indicesString);
		int index;
		while (!ss.eof())
		{
			ss >> index;
			positionIndices.push_back(index);
			ss >> index;
			ss >> index;
			textureIndices.push_back(index);
		}

		orderArray(positions, positionIndices, 3);
		orderArray(texCoords, textureIndices, 2);
	}

	void Parser::extractLineContent(std::fstream &file, std::string &content, const char *linePattern, const char* firstDelim, const char* lastDelim)
	{
		ignoreLinesUntil(linePattern, file);
		int pos = getNextLineStreamPos(file);
		char* line = new char[pos];
		file.read(line, pos);
		content = std::string(line);
		content = content.substr(content.find(firstDelim) + std::string(firstDelim).size());
		content.erase(content.find(lastDelim));
		delete[] line;
	}

	int Parser::getNextLineStreamPos(std::fstream &file)
	{
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::streamsize pos = file.gcount();
		if (file.eof())
			file.clear();
		file.seekg(-pos, std::ios_base::cur);
		return pos;
	}

	void Parser::ignoreLinesUntil(const char* pattern, std::fstream& file)
	{
		while (file)
		{
			int pos = getNextLineStreamPos(file);
			char *line = new char[pos];
			file.read(line, pos);
			if (std::string(line).find(pattern) != std::string::npos)
			{
				file.seekg(-pos, std::ios_base::cur);
				break;
			}
			delete[] line;
		}
	}

	void Parser::orderArray(std::vector<float>& array, std::vector<int>& indexBufferArray, int stride)
	{
		std::vector<float> orderedArray;
		orderedArray.resize(indexBufferArray.size() * stride);
		for (int i = 0; i < indexBufferArray.size(); i++)
		{
			for (int a = 0; a < stride; a++)
			{
				orderedArray[i * stride + a] = array[indexBufferArray[i] * stride + a];
			}
		}
		array.swap(orderedArray);
	}

	void Parser::clearIndexArray(std::vector<int>& indexArray)
	{
		std::vector<int> clearedIndexArray;
		for (int i = 0; i < indexArray.size(); i++)
		{
			if (std::find(clearedIndexArray.begin(), clearedIndexArray.end(), indexArray[i]) == clearedIndexArray.end())
			{
				clearedIndexArray.push_back(indexArray[i]);
			}
		}
		indexArray.swap(clearedIndexArray);
	}

	std::string Parser::readFile(const char* filepath)
	{
		std::fstream file(filepath);
		assertNoAbort(file.is_open(), "could not open file " + std::string(filepath));

		std::stringstream ss;
		ss << file.rdbuf();
		return ss.str();
	}
}