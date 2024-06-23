#include "Generic/ECS/EntityManager.h"

void Generic::EntityManager::removeEntity(int entityId)
{
	ArchetypeManager::removeEntity(entityId);
}

std::map<Generic::VLUI64, int> Generic::EntityManager::entityTypeIDs;