#ifndef SYSTEM_H
#define SYSTEM_H

#include <functional>
#include <unordered_map>
#include "Generic/Util/RTTI.h"
#include "Generic/ECS/ArchetypeManager.h"

namespace Generic {
	class System
	{
	public:
		virtual void init() = 0;
		virtual void run() = 0;
		void runEntityProcs() {
			for (auto pair : entityProcs)
			{
				pair.second();
			}
		}

		template <typename T, typename ...Types>
		void addEntityProc(int procName, std::function<void(std::unordered_map<int, Component*>&)> &f)
		{
			std::vector<int> archetypes;
			ArchetypeManager::getArchetypesWithComponents({GRTTI::_typeId<T>(), GRTTI::_typeId<Types>()... }, archetypes);
			entityProcs.insert({ procName, [f]() {
				for (int archetype : archetypes)
				{
					ArchetypeManager::archetypes[archetype].iterateEntities(f);
				}
				} });
		}

	private:
		std::unordered_map<int, std::function<void()>> entityProcs;
	};
}

#endif