#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Generic/Util/VLUI64.h"
#include "Generic/Util/RTTI.h"
#include "Generic/ECS/ArchetypeManager.h"
#include "Generic/Core/Logger.h"
#include "Generic/ECS/ComponentManager.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/Util/Util.h"
#include <unordered_map>

namespace Generic{
	class EntityManager
	{
	public:
		template <typename T, typename ...Types>
		static VLUI64 entityTypeMask() {
			if constexpr (sizeof...(Types) != 0)
				static VLUI64 entityTypeMask = (ComponentManager::typeMask<Types>() | ...) | ComponentManager::typeMask<T>();
			else
				static VLUI64 entityTypeMask = ComponentManager::typeMask<T>();
			return entityTypeMask;
		}

		static VLUI64 entityTypeMask(int entityTypeId) {
			assertNoAbort([&entityTypeId]()->bool {return entityTypeMasks.find(entityTypeId) != entityTypeMasks.end(); }, 
				"EntityManager :: entityTypeMask :: entityTypeId not found");
			return entityTypeMasks[entityTypeId];
		}

		template <typename T, typename ...Types>
		static int entityTypeId() {
			static int typeID = (entityTypeMasks[entityTypeIDCount] = entityTypeMask<T, Types...>(), entityTypeIDCount++);
			return typeID;
		}

		template <typename T, typename ...Types>
		static int addEntity() {
			return addEntity(entityTypeId<T, Types...>());
		}

		static const int maxEntityTypeCount = 40;

	private:
		static int addEntity(int entityTypeId);
		static void removeEntity(int entityId, int entityTypeId);

		static int entityTypeIDCount;
		static std::unordered_map<int, VLUI64> entityTypeMasks;
	};
}

#endif 