#include "Generic/ECS/ArchetypeManager.h"
#include "Generic/Util/Util.h"
#include "Generic/ECS/EntityManager.h"

void Generic::ArchetypeManager::removeEntity(int entityId) {
    assertNoAbort([&entityId]()->bool {return entityLocations.find(entityId) != entityLocations.end(); },
        "ArchetypeManager :: removeEntity :: Entity with ID " + std::to_string(entityId) + " not found");

    const auto& location = entityLocations[entityId];
    int entityTypeId = location.first;
    int entityLocalId = location.second;

    assertNoAbort([&entityTypeId]()->bool {return entityTypeId < Generic::EntityManager::maxEntityTypeCount; },
        "ArchetypeManager :: removeEntity :: entity type ID " + std::to_string(entityTypeId) + "out of bounds,"
        " you may increase maximum entity type count if required.");

    archetypes[entityTypeId].removeEntity(entityTypeId, entityLocalId);

    entityLocations.erase(entityId);
	entityIDsAllocator.returnName(entityId);
}

Component* Generic::ArchetypeManager::getComponent(const int& entityId, const int& componentTypeId)
{
    int entityType = entityLocations[entityId].first;
    int entityLocalId = entityLocations[entityId].second;
    return archetypes[entityType].getComponent(entityLocalId, componentTypeId, std::move(entityTypeNames[entityType]));
}

void Generic::ArchetypeManager::addEntityTypeName(const int& entityTypeId, std::string &&name)
{
    entityTypeNames[entityTypeId] = name;
}

bool Generic::ArchetypeManager::entityTypeExists(const int& entityTypeId) {
    return archetypes.find(entityTypeId) != archetypes.end();
}

//(archetypes[_archetype].linked == true) ? linkArchetype(archetypes[_archetype]) : 

int Generic::ArchetypeManager::entityTypeIDCount = 0;
Generic::NameAllocator Generic::ArchetypeManager::typeIDsAllocator = Generic::NameAllocator(Generic::EntityManager::maxEntityTypeCount);
std::unordered_map<int, std::vector<int>> Generic::ArchetypeManager::componentTypeIdsList;
std::unordered_map<int, Generic::Archetype> Generic::ArchetypeManager::archetypes;
Generic::NameAllocator Generic::ArchetypeManager::entityIDsAllocator = Generic::NameAllocator(maxEntityCount);
std::unordered_map<int, std::pair<int, int>> Generic::ArchetypeManager::entityLocations;
std::unordered_map<int, std::string> Generic::ArchetypeManager::entityTypeNames;
std::unordered_map<int, Generic::VLUI64> Generic::ArchetypeManager::entityTypeMasks;
Generic::SharedRecord Generic::ArchetypeManager::superArchetypesArchive;
Generic::Record Generic::ArchetypeManager::archetypeRecord;
