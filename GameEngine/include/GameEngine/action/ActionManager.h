#ifndef ACTION_MANAGER_H
#define ACTION_MANAGER_H

#include <unordered_map>
#include <functional>
#include <any>

namespace Gen
{
	class ActionManager
	{
	public:
		static void addAction(int actionEnum, std::function<void(std::any data, int entityId)> action);
		static void runAction(int actionEnum, std::any data, int entityId);

	private:
		static std::unordered_map<int, std::function<void(std::any, int)>> actions;
	};
}

#endif 
