#include "Generic/ECS/EntityManager.h"

int Generic::EntityManager::addEntity(int entityTypeId) {
	return ArchetypeManager::addEntity(entityTypeId);
}

void Generic::EntityManager::removeEntity(int entityId, int entityTypeId) {
	return ArchetypeManager::removeEntity(entityId);
}

int Generic::EntityManager::entityTypeIDCount = 0;
std::unordered_map<int, Generic::VLUI64> Generic::EntityManager::entityTypeMasks;