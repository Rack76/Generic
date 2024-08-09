#ifndef INPUT_H
#define INPUT_H

#include "GameEngine/util/Singleton.h"
#include "GameEngine/ECS/System.h"

namespace Gen
{
	class Input : public Singleton<Input>, public System
	{
	public:
		void init();
		void run();
	};
}

#endif 