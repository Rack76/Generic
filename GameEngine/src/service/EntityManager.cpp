#include "GameEngine/service/EntityManager.h"

namespace Gen
{
	int EntityManager::entityTypeCount = 0;
	Tree<int> EntityManager::typeIdTree;
	std::map<std::vector<int>, int> EntityManager::typeIds;
}