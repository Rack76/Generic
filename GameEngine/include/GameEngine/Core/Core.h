#ifndef CORE_H
#define CORE_H

class Window;
class Input;
class Renderer;

#include "GameEngine/Util/Clock.h"

namespace Gen
{
	class Core
	{
	public:
		static void init();
		static void run();

	private:
		static Window *window;
		static Input *input;
		static Renderer *renderer;
		static Clock clock;
	};
}

#endif