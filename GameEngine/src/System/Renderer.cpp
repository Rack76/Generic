#include "GL/glew.h"
#include "GameEngine/System/Renderer.h"

namespace gen
{
	void Renderer::init()
	{
		glewInit();
	}

	void Renderer::run()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}