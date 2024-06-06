#include "Generic/ECS/ArchetypeManager.h"
#include "Generic/Util/Util.h"
#include "Generic/ECS/EntityManager.h"

int Generic::ArchetypeManager::addEntity(int entityTypeId) {
	int entityId = entityIDsAllocator.getName();
	assertNoAbort([&entityTypeId]()->bool {return entityTypeId < EntityManager::maxEntityTypeCount; }, "ArchetypeManager :: entity type id out of bounds");
	int entityIndex = archetypes[entityTypeId].addEntity(entityTypeId);
	entityLocations[entityId] = {entityTypeId, entityIndex};
	return entityId;
}

void Generic::ArchetypeManager::removeEntity(int entityId) {
    assertNoAbort([&entityId]()->bool {return entityLocations.find(entityId) != entityLocations.end(); },
        "ArchetypeManager :: removeEntity :: Entity with ID " + std::to_string(entityId) + " not found");

    const auto& location = entityLocations[entityId];
    int entityTypeId = location.first;
    int entityIndex = location.second;

    assertNoAbort([&entityTypeId]()->bool {return entityTypeId < Generic::EntityManager::maxEntityTypeCount; },
        "ArchetypeManager :: removeEntity :: entity type ID " + std::to_string(entityTypeId) + "out of bounds,"
        " you may increase maximum entity type count if required.");

    archetypes[entityTypeId].removeEntity(entityIndex, entityTypeId);

    entityLocations.erase(entityId);
	entityIDsAllocator.returnName(entityId);
}

Generic::NameAllocator Generic::ArchetypeManager::typeIDsAllocator = Generic::NameAllocator(Generic::EntityManager::maxEntityTypeCount);
std::unordered_map<int, std::vector<int>> Generic::ArchetypeManager::componentTypeIdsList;
Generic::Archetype Generic::ArchetypeManager::archetypes[Generic::EntityManager::maxEntityTypeCount];
Generic::NameAllocator Generic::ArchetypeManager::entityIDsAllocator = Generic::NameAllocator(maxEntityCount);
std::unordered_map<int, std::pair<int, int>> Generic::ArchetypeManager::entityLocations;
