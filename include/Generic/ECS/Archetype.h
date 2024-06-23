#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include <unordered_map>
#include <vector>
#include <functional>
#include "Generic/ECS/ComponentManager.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/Util/VLUI64.h"
#include "Generic/Util/RTTI.h"

namespace Generic {
	class Archetype
	{
	public:
		int addEntity(const int& entityTypeId);
		void removeEntity(const int& entityTypeId, const int& entityLocalId);
		void addSuperArchetype(const int& level, const int &link, const int &entityTypeId);
		void addSubArchetype(const int& level, const int& link, const int& entityTypeId);
		Component* getComponent(const int& entityId, const int& componentTypeId, std::string&&);
		void getArchetypeRecurse(VLUI64 &excluded, std::vector<Archetype*> &archetypes);

		void iterateEntities(std::function<void(std::unordered_map<int, Component*>&)> &f)
		{
			for (auto &pair2 : entityIdAllocator.data())
			{
				std::unordered_map<int, Component*> entity;
				for (auto &pair1 : entities)
				{
					entity.insert({ pair1.first, pair1.second[pair2.first] });
				}
				f(entity);
			}
		}

		template <typename T>
		Component* getComponent(const int& entityId)
		{
			assertNoAbort([this]()->bool {return entities.find(GRTTI::_typeId<T>()) != entities.end(); },
				"Archetype :: getComponent :: component " + GRTTI::typeName<T>() + " not found in archetype");
			return entities[GRTTI::_typeId<T>()][entityId];
		}

		template <typename T, typename ...Types>
		int addEntityWithArgs(const int& entityTypeId, 
					  T &&arg,
					  Types &&...args) {
			entityCount++;
			int entityId = entityIdAllocator.getName();
			assertNoAbort([&entityId]()->bool {return entityId < maxEntityCountPerArchetype; },
				"Archetype :: addEntity :: maximum entity count reached :: you may increase maximum entity count for all archetypes");
			getComponents(entityTypeId, entityId, std::move(arg), std::move(args)...);
			return entityId;
		}

		int addEntity(const int& entityTypeId, std::unordered_map<int, Component&&> &&args){
			entityCount++;
			int entityId = entityIdAllocator.getName();
			assertNoAbort([&entityId]()->bool {return entityId < maxEntityCountPerArchetype; },
				"Archetype :: addEntity :: maximum entity count reached :: you may increase maximum entity count for all archetypes");
			for (auto& pair : args)
			{
				int componentTypeId = pair.first;
				entities[componentTypeId][entityId] = ComponentManager::getComponentWithArg(entityTypeId, componentTypeId, std::move(pair.second));
			}
			return entityId;
		}

		void transferEntity(const int& entityTypeId, const int& destArchetype, const int& entityId, std::unordered_map<int, Component&&> &&components);

		VLUI64 mask;
		int id;
		std::vector<int> signature;

	private:
		template <typename T, typename ...Types>
		void getComponents(const int& entityTypeId, const int& entityId, T&& arg, Types &&...args) {
			entities[GRTTI::_typeId<T>()][entityId] = ComponentManager::getComponentWithArgs(entityTypeId, std::move(arg));
			if constexpr (sizeof...(Types) != 0)
				getComponents(entityTypeId, entityId, std::move(args)...);
		}

		static const int maxEntityCountPerArchetype = 20;
		NameAllocator entityIdAllocator = NameAllocator(maxEntityCountPerArchetype);
		std::unordered_map<int, Component*[maxEntityCountPerArchetype]> entities;
		std::unordered_map<int, std::unordered_map<int, int>> superArchetypes;
		std::unordered_map<int, std::unordered_map<int, int>> subArchetypes;
		int entityCount = 0;
	};
}

#endif 