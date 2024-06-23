#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <unordered_map>
#include <string>
#include <cassert>
#include "Generic/Util/RTTI.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/ECS/Component.h"
#include "Generic/Core/Logger.h"
#include "Generic/ECS/PoolAllocator.h"
#include "Generic/Util/Util.h"

namespace Generic {
	class ComponentManager {
	public:
		static void alloc() {
			for (auto &poolAllocator : poolAllocators)
			{
				poolAllocator.second->alloc();
			}
		}

		template <typename T>
		static void registerComponentType() {
			assertNoAbort([]()->bool {return std::is_base_of<Component, T>::value; }, 
				"ComponentManager :: registerComponentType :: " + GRTTI::typeName<T>() + " doesnt inherit from Component");
			componentTypeSizes[GRTTI::_typeId<T>()] = sizeof(T);
			poolAllocators[GRTTI::_typeId<T>()] = std::make_unique<PoolAllocatorTemplate<T>>();
		}

		template <typename T>
		static Component* getComponentWithArgs(const int& entityTypeId, T &&arg) noexcept {
			assertNoAbort([&entityTypeId]()->bool {return poolAllocators.find(GRTTI::_typeId<T>()) != poolAllocators.end(); }, "ComponentManager :: getComponent :: "
				+ GRTTI::typeName<T>() + " is not part of this entityType");
			if (poolAllocators.find(GRTTI::_typeId<T>()) == poolAllocators.end())
				return nullptr;
			assertNoAbort([&entityTypeId]()->bool {return !poolAllocators[GRTTI::_typeId<T>()]->isEmpty(); }, "ComponentManager :: getComponent :: pool empty,"
				" ensure ComponentManager::alloc is called right after all component types registrations");
			Component* c = poolAllocators[GRTTI::_typeId<T>()]->getComponentWithArg(entityTypeId, std::move(arg));
			c->reset();
			return c;
		}

		static Component* getComponentWithArg(const int& entityTypeId, int componentTypeId, Component&& arg) noexcept {
			assertNoAbort([&, entityTypeId, componentTypeId]()->bool {return poolAllocators.find(componentTypeId) != poolAllocators.end(); }, 
				"ComponentManager :: getComponent with arg :: this compoennt type is not part of this entityType");
			if (poolAllocators.find(componentTypeId) == poolAllocators.end())
				return nullptr;
			assertNoAbort([&, entityTypeId, componentTypeId]()->bool {return !poolAllocators[componentTypeId]->isEmpty(); }, "ComponentManager :: getComponent :: pool empty,"
				" ensure ComponentManager::alloc is called right after all component types registrations");
			Component* c = poolAllocators[componentTypeId]->getComponentWithArg(entityTypeId, std::move(arg));
			c->reset();
			return c;
		}

		static Component* getComponent(const int& entityTypeId, const int& componentTypeId) {
			assert(!poolAllocators[componentTypeId]->isEmpty() && "ComponentManager :: getComponent :: pool empty,"
			" ensure ComponentManager::alloc is called right after all component types registrations");
			Component* c = poolAllocators[componentTypeId]->getComponent(entityTypeId);
			c->reset();
			return c;
		}

		static void returnComponent(const int& entityTypeId, const int& componentTypeId, Component* c) {
			poolAllocators[componentTypeId]->returnComponent(entityTypeId, c);
		}

		static void returnComponentWithCopy(const int& entityTypeId, const int& componentTypeId, Component* c) {
			poolAllocators[componentTypeId]->returnComponentWithCopy(entityTypeId, c);
		}

		static std::unordered_map<int, int> componentTypeSizes;
		static std::unordered_map<int, std::string> componentTypeNames;

	private:
		static int componentTypeIDCount;
		static std::unordered_map<int, std::unique_ptr<PoolAllocator>> poolAllocators;
	};
}

#endif 