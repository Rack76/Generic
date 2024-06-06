#include "Generic/ECS/Archetype.h"
#include "Generic/ECS/ComponentManager.h"
#include "Generic/ECS/EntityManager.h"

int Generic::Archetype::addEntity(int entityTypeId) {
	int entityId = entityIdAllocator.getName();
	auto &&entityTypeMask = EntityManager::entityTypeMask(entityTypeId);
	entityTypeMask.iterateBits([&, entityId, entityTypeId, this](int componentTypeId) {
		entities[componentTypeId][entityId] = ComponentManager::getComponent(entityTypeId, componentTypeId); });
	return entityId;
}

void Generic::Archetype::removeEntity(int entityId, int entityTypeId) {
	entities.erase(entityId);
	//TODO erase components asociated with this entity
	entityIdAllocator.returnName(entityId);
}