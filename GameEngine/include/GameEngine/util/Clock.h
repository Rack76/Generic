#ifndef CLOCK_H
#define CLOCK_H

#include "GLFW/glfw3.h"

namespace Gen
{
	class Clock
	{
	public:
		void reset() {
			referenceTime = glfwGetTime();
		}

		float getTime() {
			return glfwGetTime() - referenceTime;
		}

	private:
		float referenceTime = 0;
	};
}

#endif 