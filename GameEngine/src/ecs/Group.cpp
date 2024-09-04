#include "GameEngine/ecs/Group.h"
#include <algorithm>

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

	std::vector<Archetype*> Group::getArchetypesWith(const std::set<int> &include, const std::set<int> &exclude)
	{
		std::vector<Archetype*> returnValue;

		int typeId;
		if (!entityTypeExists(include))
			typeId = addEntityType(std::move(include));
		else
			typeId = getTypeId(include);

		archetypes[typeId].getSuperArchetypes(returnValue, exclude);
		
		return returnValue;
	}
}