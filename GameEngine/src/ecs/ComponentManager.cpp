#include "GameEngine/ecs/ComponentManager.h"

namespace Gen
{
	void ComponentManager::returnComponent(const int componentTypeId, int entityTypeId, Component *component)
	{
		assertNoAbort(poolAllocators.find(componentTypeId) != poolAllocators.end(),
			"ComponentManager :: returnComponent : component type not registered");
		poolAllocators[componentTypeId].returnComponent(entityTypeId, component);
	}

	std::unordered_map<int, PoolAllocator> ComponentManager::poolAllocators;
}