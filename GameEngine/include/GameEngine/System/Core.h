#ifndef CORE_H
#define CORE_H

#include "GameEngine/util/Singleton.h"
class Window;
class Input;
class Renderer;

namespace Gen
{
	class Core : public Singleton<Core>
	{
	public:
		template <
			typename _Window,
			typename _Input,
			typename _Renderer>
		Core() {
			window = _Window::getInstance();
			input = _Input::getInstance();
			renderer = _Renderer::getInstance();
		}
		virtual void init();
		virtual void run();

	private:
		Window *window;
		Input *input;
		Renderer *renderer;
	};
}

#endif