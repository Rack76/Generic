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
		void addAction(int trigger, int actionEnum);

		void runActions(int trigger, std::any data = 0);

	private:
		std::map<int, std::vector<int>> actions;
	};
}

#endif
