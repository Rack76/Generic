#include "GameEngine/action/ActionManager.h"
#include "GameEngine/action/ActionManager.h"

namespace Gen
{
	void ActionManager::addAction(int actionEnum, std::function<void(std::any data, int entityId)> action)
	{
		actions[actionEnum] = action;
	}
	void Gen::ActionManager::runAction(int actionEnum, std::any data, int entityId)
	{
		actions[actionEnum](data, entityId);
	}

	std::unordered_map<int, std::function<void(std::any, int)>> ActionManager::actions;
}