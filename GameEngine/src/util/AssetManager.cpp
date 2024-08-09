#include "GameEngine/Util/AssetManager.h"
#include "GameEngine/Util/Parser.h"
#include "GameEngine/System/Renderer.h"
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


namespace Gen
{
	void AssetManager::Model::loadModel(std::string name, const char* path)
	{
		std::vector<float> positions;
		std::vector<float> texCoords;

		Parser::getModelData(path, positions, texCoords);

		uint32_t positionsBuffer = Renderer::Buffer::create();
		uint32_t texCoordsBuffer = Renderer::Buffer::create();

		Renderer::Buffer::fill(positionsBuffer, 0, 3, positions);
		Renderer::Buffer::fill(texCoordsBuffer, 1, 2, texCoords);

		uint32_t vertexAttributesArray = Renderer::BufferArray::create();

		Renderer::BufferArray::bind(vertexAttributesArray);

		Renderer::Buffer::bind(positionsBuffer, 0, 3);
		Renderer::Buffer::bind(texCoordsBuffer, 1, 2);

		vertexAttributesArrays[name] = vertexAttributesArray;
		vertexCounts[name] = positions.size() / 3;
	}

	uint32_t AssetManager::Model::getMesh(std::string &name)
	{
		return vertexAttributesArrays[name];
	}

	int AssetManager::Model::getVertexCount(std::string& name)
	{
		return vertexCounts[name];
	}

	void AssetManager::Shader::loadShaderProgram(std::string &&name, const char* vertexShaderPath, const char* fragmentShaderPath)
	{
		std::string &&vertexShaderSource = Parser::readFile(vertexShaderPath);
		std::string &&fragmentShaderSource = Parser::readFile(fragmentShaderPath);

		uint32_t vertexShader = Renderer::Shader::createShader(vertexShaderSource, VERTEX);
		uint32_t fragmentShader = Renderer::Shader::createShader(fragmentShaderSource, FRAGMENT);

		uint32_t shaderProgram = Renderer::Shader::createProgram(vertexShader, fragmentShader);
		shaderPrograms[name] = shaderProgram;
	}

	uint32_t AssetManager::Shader::getShaderProgram(std::string& name)
	{
		return shaderPrograms[name];
	}

	void AssetManager::Shader::setUniform(const char* uniformName, const float *value)
	{
		for (auto pair : shaderPrograms)
		{
			Renderer::Shader::setUniform(pair.second, uniformName, value);
		}
	}

	void AssetManager::Texture::loadTexture(const char* name, const char* path)
	{
		int x, y, n;
		unsigned char* data = stbi_load(path, &x, &y, &n, 4);

		assertNoAbort(data, "could not find image " + std::string(path));

		uint32_t texture = Renderer::Texture::createTexture();
		Renderer::Texture::fillTexture(texture, 0, x, y, data);
		stbi_image_free(data);

		textures[name] = texture;
	}

	uint32_t AssetManager::Texture::getTexture(std::string& name)
	{
		return textures[name];
	}

	std::unordered_map<std::string, uint32_t> AssetManager::Model::vertexAttributesArrays;
	std::unordered_map<std::string, uint32_t> AssetManager::Model::positionsArrays;
	std::unordered_map<std::string, uint32_t> AssetManager::Model::texCoordsArrays;
	std::unordered_map<std::string, uint32_t> AssetManager::Model::indicesArrays;
	std::unordered_map<std::string, int> AssetManager::Model::vertexCounts;

	std::unordered_map<std::string, uint32_t> AssetManager::Shader::shaderPrograms;

	std::unordered_map<std::string, uint32_t> AssetManager::Texture::textures;
}