#ifndef MESH_H
#define MESH_H

#include <string>
#include "GameEngine/Util/AssetManager.h"
#include "GameEngine/Util/Util.h"
#include "GameEngine/ECS/Component.h"

namespace Gen
{
	class Mesh : public Component
	{
	public:
		Mesh()
		{

		}

		void setMesh(std::string&& name)
		{
			vao = AssetManager::Model::getMesh(name);
			meshSet = true;

			vertexCount = AssetManager::Model::getVertexCount(name);
			vertexCountSet = true;
		}

		uint32_t getMesh()
		{
			assertNoAbort(meshSet, "mesh not set");
			return vao;
		}

		int getVertexCount()
		{
			assertNoAbort(vertexCountSet, "vertex count not set");
			return vertexCount;
		}

		void setShaderProgram(std::string&& name)
		{
			shaderProgram = AssetManager::Shader::getShaderProgram(name);
			shaderProgramSet = true;
		}

		uint32_t getShaderProgram()
		{
			assertNoAbort(shaderProgramSet, "shader program not set");
			return shaderProgram;
		}

	private:
		uint32_t vao;
		bool meshSet = false;

		uint32_t shaderProgram;
		bool shaderProgramSet = false;

		int vertexCount;
		bool vertexCountSet = false;
	};
}

#endif 