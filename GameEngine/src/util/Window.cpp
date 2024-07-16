#include "GLFW/glfw3.h"
#include "GameEngine/util/Window.h"
#include "GameEngine/enum/KeyEnum.h"

namespace Gen
{
	Window::Window()
	{
		glfwInit();
		window = glfwCreateWindow(1200, 900, "Generic", nullptr, nullptr);
		glfwMakeContextCurrent(window);
	}

	void Window::setKeyCallback(void (*keyCallback)(int, int))
	{
		glfwSetWindowUserPointer(window, (void*)keyCallback);
		glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			void (*keyCallback)(int, int) = (void(*)(int, int))glfwGetWindowUserPointer(window);

			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				key = ESC_KEY;
				break;
			}

			switch (action)
			{
			case GLFW_PRESS:
				action = PRESS;
				break;
			case GLFW_RELEASE:
				action = RELEASE;
				break;
			}

			keyCallback(key, action);
			});
	}

	void Window::close()
	{
		glfwSetWindowShouldClose(window, true);
	}

	bool Window::shouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	void Window::update()
	{
		glfwSwapBuffers(window);
	}

	void Window::pollEvents()
	{
		glfwPollEvents();
	}
}