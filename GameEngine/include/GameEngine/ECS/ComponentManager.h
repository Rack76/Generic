#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "Component.h"
#include "PoolAllocator.h"
#include "GameEngine/Util/RTTI.h"
#include "GameEngine/Util/Util.h"

#include <unordered_map>

namespace Gen
{
    class ComponentManager
    {
    public:
        template <typename T>
        static void registerComponentType(int maxBlockCount = 4,
            int blockSize = 16)
        {
            poolAllocators.insert(
                { RTTI::typeId<T>(),
                 PoolAllocator(maxBlockCount, typeName<T>(), blockSize) });
            poolAllocators[RTTI::typeId<T>()].template setType<T>();
            poolAllocators[RTTI::typeId<T>()].alloc();
        }

        template <typename ComponentType>
        static Component *getComponent(const int      componentTypeId,
                                       int            entityTypeId,
                                       ComponentType *component)
        {
            assertNoAbort(poolAllocators.find(componentTypeId) != poolAllocators.end(), "component type " + typeName<ComponentType>() + " not registered");
            return poolAllocators[componentTypeId].getComponent(
                entityTypeId, reinterpret_cast<Component*>(component));
        }

        static void returnComponent(const int componentTypeId, int entityTypeId,
                                    Component *component);

        template <typename ComponentType>
        static bool isRegistered()
        {
            return (poolAllocators.find(RTTI::typeId<ComponentType>()) != poolAllocators.end());
        }

    private:
        static std::unordered_map<int, PoolAllocator> poolAllocators;
    };
}  // namespace Gen

#endif