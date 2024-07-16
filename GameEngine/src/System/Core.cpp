#include "GameEngine/System/Core.h"

namespace Gen
{
	void Core::init()
	{
		window->init();
		input->init();
		renderer->init();
	}

	void Core::run()
	{
		while (!window->shouldClose())
		{
			window->update();
			input->run();
			renderer->run();
		}
	}
}