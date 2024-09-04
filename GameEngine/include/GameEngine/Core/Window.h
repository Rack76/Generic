#ifndef WINDOW_H
#define WINDOW_H

#include "GameEngine/Util/Singleton.h"
#include "GLFW/glfw3.h"

namespace Gen
{
	class Window : public Singleton<Window>
	{
	public:
		Window();
		~Window();

		void setKeyCallback(void (*f)(int, int));
		void setCursorCallback(void (*f)(double, double, float));
		void setDT(float *dt);
		void close();
		bool shouldClose();
		void update();
		void pollEvents();
		int getHeight()
		{
			return height;
		}
		int getWidth()
		{
			return height;
		}

	private:
		GLFWwindow* window;
		int width, height;
	};
}

#endif 