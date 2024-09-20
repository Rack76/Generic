#include "GameEngine/component/PlayerInput.h"

namespace Gen
{
	void PlayerInput::addAction(int trigger, std::function<void(std::any, int)> action)
	{
		actions[trigger].push_back(action);
	}

	void PlayerInput::runActions(int trigger, std::any data)
	{
		if (actions.find(trigger) == actions.end())
			return;
		for (auto action : actions.at(trigger))
		{
			action(data, entityId);
		}
	}
}