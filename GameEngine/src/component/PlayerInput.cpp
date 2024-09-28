#include "GameEngine/component/PlayerInput.h"
#include "GameEngine/action/ActionManager.h"

namespace Gen
{
	void PlayerInput::addAction(int trigger, int actionEnum)
	{
		actions[trigger].push_back(actionEnum);
	}

	void PlayerInput::runActions(int trigger, std::any data)
	{
		if (actions.find(trigger) == actions.end())
			return;
		for (auto action : actions.at(trigger))
		{
			ActionManager::runAction(action, data, entityId);
		}
	}
}