#ifndef WINDOW_H
#define WINDOW_H

#include "GameEngine/util/Singleton.h"

class GLFWwindow;

namespace Gen
{
	class Window : public Singleton<Window>
	{
	public:
		Window();
		void setKeyCallback(void (*keyCallback)(int key, int action));
		void close();
		bool shouldClose();
		void update();
		void pollEvents();

	private:
		GLFWwindow* window;
	};
}

#endif 