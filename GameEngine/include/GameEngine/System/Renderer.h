#ifndef RENDERER_H
#define RENDERER_H

#include "GameEngine/util/Singleton.h"

namespace Gen
{
	class Renderer : public Singleton<Renderer>
	{
	public:
		void init();
		void run();
	};
}

#endif