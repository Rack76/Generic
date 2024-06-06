#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include <unordered_map>
#include <vector>
#include "Generic/ECS/Component.h"
#include "Generic/Util/NameAllocator.h"

namespace Generic {
	class Archetype
	{
	public:
		int addEntity(int entityTypeId);
		void removeEntity(int entityId, int entityTypeId);

	private:
		static const int maxEntityCountPerArchetype = 20;
		NameAllocator entityIdAllocator = NameAllocator::NameAllocator(maxEntityCountPerArchetype);
		std::unordered_map<int, Component*[maxEntityCountPerArchetype]> entities;
	};
}

#endif 