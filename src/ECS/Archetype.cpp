#include "Generic/ECS/Archetype.h"
#include "Generic/ECS/ComponentManager.h"
#include "Generic/ECS/ArchetypeManager.h"

int Generic::Archetype::addEntity(const int &entityTypeId) {
	entityCount++;
	int entityId = entityIdAllocator.getName();
	assertNoAbort([&entityId]()->bool {return entityId < maxEntityCountPerArchetype; },
		"Archetype :: addEntity :: maximum entity count reached :: you may increase maximum entity count for all archetypes");
	for (int componentTypeId : signature)
	{
		entities[componentTypeId][entityId] = ComponentManager::getComponent(entityTypeId, componentTypeId);
	}
	return entityId;
}

void Generic::Archetype::removeEntity(const int& entityTypeId, const int &entityLocalId) {
	entityCount--;
	assertNoAbort([&entityLocalId, this]()->bool {return entityIdAllocator.isReserved(entityLocalId); },
		"Archetype :: removeEntity :: cannot remove entity since it does not belong to this archetype");

	entityIdAllocator.returnName(entityLocalId);
	for (auto &componentType : entities)
	{
		ComponentManager::returnComponent(entityTypeId, componentType.first, componentType.second[entityLocalId]);
	}
}

void Generic::Archetype::addSuperArchetype(const int& level, const int& link, const int& entityTypeId) {
	superArchetypes[level][link] = entityTypeId;
}

void Generic::Archetype::addSubArchetype(const int& level, const int& link, const int& entityTypeId){
	subArchetypes[level][link] = entityTypeId;
}


Component* Generic::Archetype::getComponent(const int& entityId, const int& componentTypeId, std::string&& name)
{
	assertNoAbort([this, &componentTypeId]()->bool {return entities.find(componentTypeId) != entities.end(); },
		"Archetype :: getComponent :: no such component type found in archetype " + name);
	return entities[componentTypeId][entityId];
}

void Generic::Archetype::getArchetypeRecurse(VLUI64& excluded, std::vector<Archetype*>& archetypes)
{
	if(entityCount != 0)
		archetypes.push_back(this);
	if (superArchetypes.find(0) == superArchetypes.end())
		return;
	for (auto pair : superArchetypes[0])
	{
		VLUI64& typeMask = ArchetypeManager::entityTypeMasks[pair.second];
		if (!(typeMask & excluded).isZero())
		{
			excluded = excluded | typeMask;
			continue;
		}
		ArchetypeManager::archetypes[pair.second].getArchetypeRecurse(excluded, archetypes);
		excluded = excluded | typeMask;
	}
}

void Generic::Archetype::transferEntity(const int& entityTypeId, const int& destArchetype, const int& entityId, std::unordered_map<int, Component&&> &&components) {
	entityCount--;
	std::vector<int> signature = ArchetypeManager::archetypes[destArchetype].signature;
	for (int componentTypeId : signature)
	{
		components.insert({ componentTypeId, std::move(*entities[componentTypeId][entityId]) });
	}
	removeEntity(entityTypeId, entityId);
	ArchetypeManager::archetypes[destArchetype].addEntity(destArchetype, std::move(components));
}