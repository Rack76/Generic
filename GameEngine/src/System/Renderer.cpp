#include "glad/glad.h"
#include "GameEngine/System/Renderer.h"
#include "GameEngine/util/Logger.h"

namespace Gen
{
	void Renderer::init()
	{
		Logger logger("Renderer");
		if(!gladLoadGL())
			logger.error("glad initialzation failed : did you init glfw before ?");
	}

	void Renderer::run()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}
}