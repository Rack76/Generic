#include "GameEngine/Core/Window.h"
#include "GameEngine/enum/InputEnum.h"
#include <vector>

namespace Gen
{
	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		auto functionPointers = (std::vector<void*>*)glfwGetWindowUserPointer(window);
		auto f = (void (*)(int, int))(*functionPointers)[1];

		switch (action)
		{
		case GLFW_PRESS:
			action = PRESS;
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				key = ESC_KEY;
				break;
			case GLFW_KEY_A:
				key = A_KEY_PRESS;
				break;
			case GLFW_KEY_D:
				key = D_KEY_PRESS;
				break;
			case GLFW_KEY_SPACE:
				key = SPACE_KEY_PRESS;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				key = LEFT_SHIFT_KEY_PRESS;
				break;
			case GLFW_KEY_W:
				key = W_KEY_PRESS;
				break;
			case GLFW_KEY_S:
				key = S_KEY_PRESS;
				break;
			}
			break;
		case GLFW_RELEASE:
			action = RELEASE;
			switch (key)
			{
			case GLFW_KEY_ESCAPE:
				key = ESC_KEY;
				break;
			case GLFW_KEY_A:
				key = A_KEY_RELEASE;
				break;
			case GLFW_KEY_D:
				key = D_KEY_RELEASE;
				break;
			case GLFW_KEY_SPACE:
				key = SPACE_KEY_RELEASE;
				break;
			case GLFW_KEY_LEFT_SHIFT:
				key = LEFT_SHIFT_KEY_RELEASE;
				break;
			case GLFW_KEY_W:
				key = W_KEY_RELEASE;
				break;
			case GLFW_KEY_S:
				key = S_KEY_RELEASE;
				break;
			}
			break;
		}

		f(key, action);
	};

	void cursorCallback(GLFWwindow* window, double x, double y)
	{
		auto functionPointers = (std::vector<void*>*)glfwGetWindowUserPointer(window);
		auto f = (void (*)(double, double))(*functionPointers)[0];
		f(x, y);
		glfwSetCursorPos(window, 0, 0);
	}

	Window::Window()
	{
		glfwInit();
		window = glfwCreateWindow(1200, 900, "Generic", nullptr, nullptr);
		glfwMakeContextCurrent(window);
		glfwGetWindowSize(window, &width, &height);
		auto functionPointers = new std::vector<void*>;
		glfwSetWindowUserPointer(window, (void*)functionPointers);
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	Window::~Window()
	{
		auto functionPointers = (std::vector<void*>*)glfwGetWindowUserPointer(window);
		delete functionPointers;
		glfwTerminate();
	}

	void Window::setKeyCallback(void (*f)(int, int))
	{
		auto functionPointers = (std::vector<void*>*)glfwGetWindowUserPointer(window);
		functionPointers->push_back(f);
		glfwSetKeyCallback(window, keyCallback);
	}

	void Window::setCursorCallback(void (*f)(double, double))
	{
		auto functionPointers = (std::vector<void*>*)glfwGetWindowUserPointer(window);
		functionPointers->push_back(f);
		glfwSetCursorPosCallback(window, cursorCallback);
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