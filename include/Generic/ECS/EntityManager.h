#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Generic/Util/VLUI64.h"
#include "Generic/Util/RTTI.h"
#include "Generic/ECS/ArchetypeManager.h"
#include "Generic/Core/Logger.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/Util/Util.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <functional>
#include <string>

namespace Generic{

	using ComponentTypeId = int;
	using EntityTypeId = int;

	class EntityManager
	{
	public:

		template <typename T, typename ...Types>
		static int addEntityType() {
			return ArchetypeManager::addEntityType<T, Types...>();
		}

		static void setEntityTypeName(const int& entityTypeId, std::string&& name) {
			ArchetypeManager::addEntityTypeName(entityTypeId, std::move(name));
		}

		template <typename T, typename ...Types>
		static int addEntity(T &&arg, Types &&...args) {
			return ArchetypeManager::addEntity(std::move(arg), std::move(args)...);
		}

		template <typename T, typename ...Types>
		static int addEntity() {
			return ArchetypeManager::addEntity<T, Types...>();
		}

		static void removeEntity(int entityId);

		template <typename T, typename ...Types>
		static void addComponents(const int &entityId, T&& component, Types &&..._components) {
			ArchetypeManager::addComponents<T, Types...>(entityId, std::move(component), std::move(_components)...);
		}

		template <typename T>
		static T* getComponent(int &entityId) {
			return (T*)ArchetypeManager::getComponent(entityId, GRTTI::_typeId<T>());
		}

		static void getArchetypesWithComponents(std::vector<int>&& include, std::unordered_map<int, int>& archetypes, std::vector<int>&& exclude = {}) {
			ArchetypeManager::getArchetypesWithComponents(std::move(include), archetypes, std::move(exclude));
		}

		static const int maxEntityTypeCount = 40;
		static int entityTypeIDCount;

	private:

		static std::map<VLUI64, int> entityTypeIDs;
	};
}

#endif 