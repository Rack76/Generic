#include "glad/glad.h"
#include "GameEngine/System/Renderer.h"

namespace Gen
{
	void Renderer::init()
	{
		gladLoadGL();
	}

	void Renderer::run()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}