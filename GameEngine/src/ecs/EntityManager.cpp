#include "GameEngine/ecs/EntityManager.h"

namespace Gen
{
	int EntityManager::activeGroup = 0;
	std::unordered_map<int, Group> EntityManager::groups;
}