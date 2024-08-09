#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

#include <any>
#include <functional>
#include <map>
#include <memory>
#include <iostream>
#include "GameEngine/ECS/Component.h"

namespace Gen
{
	class PlayerInput : public Component
	{
	public:
		PlayerInput()
		{

		}

		void addAction(int trigger, std::function<void(std::any, int)> action)
		{
			actions[trigger].push_back(action);
		}

		void runActions(int trigger, std::any data = 0)
		{
			if (actions.find(trigger) == actions.end())
				return;
			for (auto action : actions.at(trigger))
			{
				action(data, entityId);
			}
		}

	private:
		std::map<int, std::vector<std::function<void(std::any, int)>>> actions;
	};
}

#endif