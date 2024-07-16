#ifndef INPUT_H
#define INPUT_H

#include "GameEngine/util/Singleton.h"

namespace Gen
{
	class Input : public Singleton<Input>
	{
	public:
		void init();
		void run();
	};
}

#endif 