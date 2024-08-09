#include "GameEngine/System/Renderer.h"
#include "GameEngine/util/Util.h"
#include "GameEngine/ECS/EntityManager.h"
#include "GameEngine/Util/RTTI.h"
#include "GameEngine/component/Mesh.h"
#include "GameEngine/enum/EntityProcRenderer.h"
#include "GameEngine/component/Camera.h"
#include "GameEngine/component/Texture.h"

namespace Gen
{
	void Renderer::init()
	{
		int result = glewInit();
		assertNoAbort(result == GLEW_OK, "Renderer::init : glew initialization failed");

		addEntityProc(
			DRAW_MONOCHROMATIC,
			[](Entity&& entity) {
				uint32_t vao = entity.get<Mesh>()->getMesh();
				glBindVertexArray(vao);

				uint32_t shaderProgram = entity.get<Mesh>()->getShaderProgram();
				glUseProgram(shaderProgram);

				int vertexCount = entity.get<Mesh>()->getVertexCount();

				glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			},
			{RTTI::typeId<Mesh>() }, 
			{RTTI::typeId<Gen::Texture>()}
			);

		addEntityProc(
			DRAW_TEXTURE,
			[](Entity&& entity) {
				uint32_t vao = entity.get<Mesh>()->getMesh();
				glBindVertexArray(vao);

				uint32_t shaderProgram = entity.get<Mesh>()->getShaderProgram();
				glUseProgram(shaderProgram);

				uint32_t textureObject = entity.get<Gen::Texture>()->getTextureObject();
				glBindTexture(GL_TEXTURE_2D, textureObject);

				int vertexCount = entity.get<Mesh>()->getVertexCount();

				glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			},
			{ RTTI::typeId<Mesh>(), RTTI::typeId<Gen::Texture>()});

		assertNoAbort(false, std::to_string(glGetError()));

		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_DEPTH_TEST);
	}

	void Renderer::run()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(Camera::getActiveCamera())
			Camera::getActiveCamera()->update();
		runEntityProcs();
	}

	uint32_t Renderer::Buffer::create()
	{
		GLuint VBO;
		glGenBuffers(1, &VBO);
		return VBO;
	}

	void Renderer::Buffer::bind(const uint32_t& vbo, int&& index, int&& size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(index);
	}

	void Renderer::Buffer::fill(const uint32_t& vbo, int &&index, int &&size, const std::vector<float> &data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * 4, data.data(), GL_STATIC_DRAW);
	}

	uint32_t Renderer::BufferArray::create()
	{
		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		return VAO;
	}

	void Renderer::BufferArray::bind(const uint32_t &vao)
	{
		glBindVertexArray(vao);
	}

	uint32_t Renderer::Shader::createShader(std::string &shaderString, int TYPE)
	{
		switch (TYPE)
		{
		case VERTEX:
			TYPE = GL_VERTEX_SHADER;
			break;
		case FRAGMENT:
			TYPE = GL_FRAGMENT_SHADER;
			break;
		}

	    uint32_t shader = glCreateShader(TYPE);
		const char* shaderCString = shaderString.c_str();
		glShaderSource(shader, 1, &shaderCString, nullptr);
		glCompileShader(shader);

		int result;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLchar* errorMessage = new GLchar[500];
			glGetShaderInfoLog(shader, 500, nullptr, errorMessage);
			std::cout << errorMessage << std::endl;
			delete[] errorMessage;
		}

		return shader;
	}

	void Renderer::Shader::setUniform(uint32_t program, const char* uniformName, const float* value)
	{
		glUseProgram(program);
		GLint location = glGetUniformLocation(program, uniformName);
		glUniformMatrix4fv(location, 1,GL_FALSE, value);
	}

	uint32_t Renderer::IndexBuffer::create()
	{
		GLuint EBO;
		glGenBuffers(1, &EBO);
		return EBO;
	}

	void Renderer::IndexBuffer::bind(const uint32_t& ebo)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	void Renderer::IndexBuffer::fill(const uint32_t& ebo, const std::vector<int>& data)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * 4, data.data(), GL_STATIC_DRAW);
	}

	uint32_t Renderer::Texture::createTexture()
	{
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		return texture;
	}

	void Renderer::Texture::fillTexture(int texture, int mipmapLevels, int width, int height, unsigned char* data)
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, mipmapLevels, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
}