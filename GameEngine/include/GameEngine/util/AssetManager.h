#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <unordered_map>
#include "GameEngine/Core/File.h"
#include "glm/glm.hpp"

namespace Gen
{
	class AssetManager
	{
	public:
		class Model
		{
		public:
			static void loadModel(std::string name, const char* path);
			static uint32_t getMesh(std::string &name);
			static int getVertexCount(std::string& name);

		private:
			static std::unordered_map<std::string, uint32_t> vertexAttributesArrays;
			static std::unordered_map<std::string, std::vector<std::array<glm::vec3, 3>>> triangles;
			static std::unordered_map<std::string, std::vector<glm::vec3>> vertices;
			static std::unordered_map<std::string, uint32_t> texCoordsArrays;
			static std::unordered_map<std::string, uint32_t> indicesArrays;
			static std::unordered_map<std::string, int> vertexCounts;
		};

		class Shader
		{
		public:
			static void loadShaderProgram(std::string &&name, const char* vertexShaderPath, const char* fragmentShaderPath);
			static uint32_t getShaderProgram(std::string& name);
			static void setUniform(int &programName, const char* uniformName, const float* value);
			static void setUniform(const char* uniformName, const float* value);

		private:
			static std::unordered_map<std::string, uint32_t> shaderPrograms;
		};

		class Texture
		{
		public:
			static void loadTexture(const char* name, const char* path);
			static uint32_t getTexture(std::string& name);

		private:
			static std::unordered_map<std::string, uint32_t> textures;
		};
	};
}

#endif 
