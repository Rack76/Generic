#include "GameEngine/Util/AssetManager.h"
#include "GameEngine/Util/Parser.h"
#include "GameEngine/System/Renderer.h"
#include <vector>
#include <array>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"


namespace Gen
{
	void AssetManager::Model::loadModel(std::string name, const char* path)
	{
		std::vector<float> triangles;
		std::vector<float> texCoords;
		std::vector<float> vertices;

		Parser::getModelData(path, triangles, texCoords, vertices);

		for (int i = 0; i < vertices.size(); i += 3)
		{
			glm::vec3 vertex = glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			AssetManager::Model::vertices[name].push_back(vertex);
		}

		for (int i = 0; i < triangles.size(); i += 9)
		{
			glm::vec3 vertex0 = glm::vec3(triangles[i], triangles[i + 1], triangles[i + 2]);
			glm::vec3 vertex1 = glm::vec3(triangles[i + 3], triangles[i + 4], triangles[i + 5]);
			glm::vec3 vertex2 = glm::vec3(triangles[i + 6], triangles[i + 7], triangles[i + 8]);
			AssetManager::Model::triangles[name].push_back({ vertex0, vertex1, vertex2 });
		}

		uint32_t trianglesBuffer = Renderer::Buffer::create();
		uint32_t texCoordsBuffer = Renderer::Buffer::create();

		Renderer::Buffer::fill(trianglesBuffer, 0, 3, triangles);
		Renderer::Buffer::fill(texCoordsBuffer, 1, 2, texCoords);

		uint32_t vertexAttributesArray = Renderer::BufferArray::create();

		Renderer::BufferArray::bind(vertexAttributesArray);

		Renderer::Buffer::bind(trianglesBuffer, 0, 3);
		Renderer::Buffer::bind(texCoordsBuffer, 1, 2);

		vertexAttributesArrays[name] = vertexAttributesArray;
		vertexCounts[name] = triangles.size() / 3;
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

	void AssetManager::Shader::setUniform(int &programName, const char* uniformName, const float* value)
	{
		Renderer::Shader::setUniform(programName, uniformName, value);
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
	std::unordered_map<std::string, std::vector<std::array<glm::vec3, 3>>> AssetManager::Model::triangles;
	std::unordered_map<std::string, std::vector<glm::vec3>> AssetManager::Model::vertices;
	std::unordered_map<std::string, uint32_t> AssetManager::Model::texCoordsArrays;
	std::unordered_map<std::string, uint32_t> AssetManager::Model::indicesArrays;
	std::unordered_map<std::string, int> AssetManager::Model::vertexCounts;

	std::unordered_map<std::string, uint32_t> AssetManager::Shader::shaderPrograms;

	std::unordered_map<std::string, uint32_t> AssetManager::Texture::textures;
}