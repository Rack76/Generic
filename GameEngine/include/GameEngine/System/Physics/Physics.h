#ifndef PHYSICS_H
#define PHYSICS_H

#include "GameEngine/Util/Singleton.h"
#include "GameEngine/ECS/System.h"

namespace Gen
{
	class Physics : public Singleton<Physics>, public System
	{
	public:
		void run(float dt);

		int substsepCount = 1;

	private:
		class CollisionDetection
		{
		public:

		};

		void moveEntities(float dt);
	};
}

#endif