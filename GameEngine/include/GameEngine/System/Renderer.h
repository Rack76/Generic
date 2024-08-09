#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glew.h"
#include "GameEngine/util/Singleton.h"
#include "GameEngine/ECS/System.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Gen
{
	enum ShaderType{VERTEX, FRAGMENT};

	class Renderer : public Singleton<Renderer>, public System
	{
	public:
		void init();
		void run();

		class BufferArray
		{
		public:
			static uint32_t create();
			static void bind(const uint32_t &vao);
		};

		class Buffer
		{
		public:
			static uint32_t create();
			static void bind(const uint32_t& vbo, int&& index, int&& size);
			static void fill(const uint32_t& vbo, int&& index, int&& size, const std::vector<float>& data);
		};

		class IndexBuffer
		{
		public:
			static uint32_t create();
			static void bind(const uint32_t& ebo);
			static void fill(const uint32_t& ebo, const std::vector<int>& data);
		};

		class Shader
		{
		public:
			template <typename ...Integers>
			static uint32_t createProgram(Integers &...shaders)
			{
				uint32_t program = glCreateProgram();
				attachShaders(program, shaders...);
				glLinkProgram(program);

				int result;
				glGetProgramiv(program, GL_LINK_STATUS, &result);

				if (result == GL_FALSE)
				{
					GLchar* errorMessage = new GLchar[100];
					glGetProgramInfoLog(program, 100, nullptr, errorMessage);
					std::cout << errorMessage << std::endl;
					delete[] errorMessage;
				}

				return program;
			}

			static uint32_t createShader(std::string& shaderString, int TYPE);

			static void setUniform(uint32_t program, const char* uniformName, const float* value);

		private:
			template <typename Integer, typename ...Integers>
			static void attachShaders(uint32_t &program, Integer &shader, Integers &...shaders)
			{
				glAttachShader(program, shader);
				//glDeleteShader(shader);
				if constexpr (sizeof...(Integers) != 0)
					attachShaders(program, shaders...);
			}
		};

		class Texture
		{
		public:
			static uint32_t createTexture();
			static void fillTexture(
				int texture,
				int mipmapLevels, 
				int width,
				int height,
				unsigned char* data);
		};
	};
}

#endif