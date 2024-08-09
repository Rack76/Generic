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

		for (int i = 0; i < signature.size(); i++)
		{
			ComponentManager::returnComponent(signature[i], id, entities[entityLocalId][signature[i]]);
		}
		entityIdsAllocator.returnName(entityLocalId);
		entities.erase(entityLocalId);
	}

	void Archetype::removeEntity(int& entityLocalId, std::unordered_map<int, Component*>& components)
	{
		for (int i = 0; i < signature.size(); i++)
		{
			components.insert({ signature[i],  entities[entityLocalId][signature[i]] });
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