#include "GameEngine/System/Renderer.h"
#include "GameEngine/System/Input.h"
#include "GameEngine/util/Window.h"
#include "GameEngine/System/Core.h"

namespace Gen
{
	void Core::init()
	{

		window = Window::getInstance();
		input = Input::getInstance();
		renderer = Renderer::getInstance();

		input->init();
		renderer->init();
	}

	void Core::run()
	{
		while (!window->shouldClose())
		{
			input->run();
			renderer->run();
			window->update();
		}
	}
}