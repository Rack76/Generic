#include "GameEngine/ecs/Group.h"

namespace Gen
{
	void Group::transferEntity(Archetype& dest, Archetype& src, int& entityId)
	{
		std::unordered_map<int, Component*> components;
		int entityLocalId = entities[entityId].second;
		src.removeEntity(entityLocalId, components);
		entityLocalId = dest.addEntity(entityId, components);
		entities[entityId] = std::make_pair(dest.id, entityLocalId);
	}

	void Group::removeEntity(int& entityId)
	{
		assertNoAbort(entities.find(entityId) != entities.end(), "Group:::removeEntity : cant remove entity : "
		"entity not found");

		int archetype = entities[entityId].first;
		int entityLocalId = entities[entityId].second;

		archetypes[archetype].removeEntity(entityLocalId);
		entityIdAllocator.returnName(entityId);
		entities.erase(entityId);
	}

	std::vector<Archetype*> Group::getArchetypesWith(const std::vector<int> &include, const std::vector<int> &exclude)
	{
		std::vector<int> signature = mergeSort(std::move(include));
		std::vector<int> archetypeIds;

		int typeId;
		typeIdTree.getElement(typeId, signature);

		std::unordered_map<int, int> excludeMap;
		for (const int& id : exclude)
		{
			excludeMap.insert({id, id});
		}
		superTypeIdsTree.getElements(archetypeIds, signature, excludeMap);
		std::vector<Archetype*> returnValue;
		if(typeId != -1)
			returnValue.push_back(&archetypes[typeId]);
		for (int& id : archetypeIds)
		{
			bool componentTypeIdFound = false;
			for (const int& componentTypeId : archetypes[id].signature)
			{
				if (excludeMap.find(componentTypeId) != excludeMap.end())
				{
					componentTypeIdFound = true;
					break;
				}
			}
			if(!componentTypeIdFound)
				returnValue.push_back(&archetypes[id]);
		}
		return returnValue;
	}
}