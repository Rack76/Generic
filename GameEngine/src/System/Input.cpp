#include "GameEngine/System/Input.h"
#include "GameEngine/util/Window.h"
#include "GameEngine/enum/KeyEnum.h"

void keyCallback(int key, int action)
{
	switch (action)
	{
	case PRESS:
		Gen::Window::getInstance()->close();
		break;

	case RELEASE:
		break;
	}
}

namespace Gen
{
	void Input::init()
	{
		Window::getInstance()->setKeyCallback(keyCallback);
	}

	void Input::run()
	{
		Window::getInstance()->pollEvents();
	}
}