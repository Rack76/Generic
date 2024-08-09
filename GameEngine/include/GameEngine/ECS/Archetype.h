#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include <vector>
#include <unordered_map>

#include "ComponentManager.h"
#include "GameEngine/Util/NameAllocator.h"
#include "GameEngine/Util/Util.h"
#include "GameEngine/Util/RTTI.h"

namespace Gen
{
    class Entity
    {
    public:
        Entity(std::unordered_map<int, Component*>&& _components)
            : components(_components)
        {
        }

        template <typename T>
        T* get()
        {
            return reinterpret_cast<T*>(components[RTTI::typeId<T>()]);
        }

    private:
        std::unordered_map<int, Component*> components;
    };

    class Archetype
    {
    public:
        Archetype() : componentTypeCount(0), id(-1)
        {
        }

        Archetype(const std::vector<int>& signature, const int& id)
            : signature(signature),
              componentTypeCount(signature.size()),
              id(id),
              entityIdsAllocator(NameAllocator(10))
        {
        }

        template <typename... ComponentTypes>
        int addEntity(int &entityId, ComponentTypes&... components)
        {
            int entityLocalId = entityIdsAllocator.getName();
            addComponents(entityId, entityLocalId, components...);
            return entityLocalId;
        }

        void removeEntity(int& entityLocalId);

        template <typename ComponentType>
        bool hasComponent(int &entityLocalId)
        {
            return (entities[entityLocalId].find(RTTI::typeId<ComponentType>()) != entities[entityLocalId].end());
        }

        template <typename ComponentType>
        ComponentType* getComponent(int& entityLocalId)
        {
            assertNoAbort(
                entities[entityLocalId].find(RTTI::typeId<ComponentType>()) !=
                    entities[entityLocalId].end(),
                "Archetype :: getComponent : component type " +
                    typeName<ComponentType>() + " not found ");

            assertNoAbort(std::is_base_of<Component, ComponentType>::value,
                          typeName<ComponentType>() +
                              " nmust inherit from Component");

            return reinterpret_cast<ComponentType*>(
                entities[entityLocalId][RTTI::typeId<ComponentType>()]);
        }

        template <typename ComponentType, typename... ComponentTypes>
        void addComponents(int &entityId, int& entityLocalId, ComponentType& component,
                           ComponentTypes&... components)
        {
            if constexpr (sizeof...(ComponentTypes) != 0)
                addComponents(entityId, entityLocalId, components...);

            int        componentTypeId = RTTI::typeId<ComponentType>();
            component.entityId = entityId;
            Component* c =
                ComponentManager::getComponent(componentTypeId, id, &component);
            c->entityId = entityId;
            entities[entityLocalId].insert({componentTypeId, c});
        }

        int addEntity(int& entityId, std::unordered_map<int, Component*>& components);
        void removeEntity(int&                                 entityLocalId,
                          std::unordered_map<int, Component*>& components);

        void iterateEntities(const std::function<void(Entity&&)>& f);

        const std::vector<int> signature;
        const int id;
        std::unordered_map<int, std::unordered_map<int, Component*>> entities;

    private:
        const int componentTypeCount;
        NameAllocator entityIdsAllocator;
    };
}  // namespace Gen

#endif