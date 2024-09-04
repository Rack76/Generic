#include "GameEngine/System/Physics/Physics.h"
#include "GameEngine/ECS/EntityManager.h"
#include "GameEngine/component/RigidBody.h"

void Gen::Physics::run(float dt)
{
	moveEntities(dt);
	collisionDetection(dt);
}

void Gen::Physics::moveEntities(float dt)
{
	std::function<void(Gen::Entity&&)> applyForces =
		[](Gen::Entity&& entity)
		{
			RigidBody* rigidBody = entity.get<RigidBody>();
			rigidBody->addForces();
		};

	int i = 0;
	while (i < substsepCount)
	{
		std::vector<Archetype*> archetypes = EntityManager::getArchetypesWith({ RTTI::typeId<RigidBody>() });
		for (Archetype* archetype : archetypes)
		{
			archetype->iterateEntities(applyForces);
		}
	}
}
