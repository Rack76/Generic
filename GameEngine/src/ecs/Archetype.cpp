#include "GameEngine/ecs/Archetype.h"

namespace Gen
{
	int Archetype::addEntity(int &entityId, std::unordered_map<int, Component*>& components)
	{
		int entityLocalId = entityIdsAllocator.getName();
		for (auto pair : components)
		{
			Component* component = ComponentManager::getComponent(pair.first, id, pair.second);
			component->entityId = entityId;
			entities[entityLocalId].insert({ pair.first, component });
		}
		return entityLocalId;
	}

	void Archetype::removeEntity(int &entityLocalId)
	{
		assertNoAbort(entities.find(entityLocalId) != entities.end(), "Group:::removeEntity : cant remove entity : "
			"entity not found");

		for (const int &componentTypeId : signature)
		{
			ComponentManager::returnComponent(componentTypeId, id, entities[entityLocalId][componentTypeId]);
		}
		entityIdsAllocator.returnName(entityLocalId);
		entities.erase(entityLocalId);
	}

	void Archetype::removeEntity(int& entityLocalId, std::unordered_map<int, Component*>& components)
	{
		for (const int &componentTypeId : signature)
		{
			components.insert({ componentTypeId,  entities[entityLocalId][componentTypeId] });
		}
		removeEntity(entityLocalId);
	}

	void Archetype::iterateEntities(const std::function<void(Entity&&)>& f)
	{
		for (auto& pair1 : entities)
		{
			f(Entity{ std::move(pair1.second) });
		}
	}
}