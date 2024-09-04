#ifndef GROUP_H
#define GROUP_H

#include "GameEngine/Util/Tree.h"
#include "GameEngine/Util/Util.h"
#include "Archetype.h"
#include "Component.h"

#include <vector>
#include <unordered_map>
#include <map>

namespace Gen
{
    class Group
    {
    public:
        template <typename ComponentType, typename... ComponentTypes>
        int addEntity(ComponentType& component, ComponentTypes&... components)
        {
            int        entityId     = entityIdAllocator.getName();
            static int entityTypeId = addEntityType(
                std::set<int>{RTTI::typeId<ComponentType>(),
                                 RTTI::typeId<ComponentTypes>()...});
            entities[entityId].first = entityTypeId;
            int entityLocalId =
                archetypes.at(entityTypeId).addEntity(entityId, component, components...);
            entities[entityId].second = entityLocalId;
            initComponents(component, components...);
            return entityId;
        }

        void removeEntity(int& entityId);

        template <typename ComponentType>
        bool hasComponent(int& entityId)
        {
            if (entityId == -1)
                return false;
            int entityLocalId = entities[entityId].second;
            int archetype = entities[entityId].first;
            return archetypes[archetype].hasComponent<ComponentType>(entityLocalId);
        }

        template <typename ComponentType>
        ComponentType* getComponent(int& entityId)
        {

            int archetypeId   = entities[entityId].first;
            int entityLocalId = entities[entityId].second;
            return archetypes[archetypeId].getComponent<ComponentType>(
                entityLocalId);
        }

        template <typename... ComponentTypes>
        void addComponents(int& entityId, ComponentTypes&... components)
        {
            std::set<int> signature = {RTTI::typeId<ComponentTypes>()...};
            int              srcArchetype  = entities[entityId].first;
            std::set<int> srcSignature  = archetypes[srcArchetype].signature;
            srcSignature.merge(signature);
            std::set<int> &destSignature = srcSignature;
            int destArchetype;
            if (!entityTypeExists(destSignature))
                destArchetype = addEntityType(std::move(destSignature));
            else
                destArchetype = getTypeId(destSignature);

            transferEntity(archetypes[destArchetype], archetypes[srcArchetype],
                           entityId);
            int entityLocalId = entities[entityId].second;
            archetypes[destArchetype].addComponents(entityId, entityLocalId,
                                                    components...);
        }

        template <typename... ComponentTypes>
        void removeComponents(int& entityId)
        {
            std::set<int> signature ={RTTI::typeId<ComponentTypes>()...};
            int              srcArchetype = entities[entityId].first;
            std::set<int> srcSignature = archetypes[srcArchetype].signature;
            std::unordered_map<int, int> signatureMap;
            for (const int& componentTypeId : signature)
            {
                signatureMap.insert({ componentTypeId, 0 });
            }
            std::set<int> destSignature;
            for (const int& componentTypeId : srcSignature)
            {
                if (signatureMap.find(componentTypeId) == signatureMap.end())
                    destSignature.insert(componentTypeId);
            }

            int destArchetype;
            if (!entityTypeExists(destSignature))
                destArchetype = addEntityType(std::move(destSignature));
            else
                destArchetype = getTypeId(destSignature);

            std::unordered_map<int, Component*> components;
            archetypes[srcArchetype].removeEntity(entityId, components);
            for (int& componentTypeId : signature)
            {
                components.erase(componentTypeId);
            }
            int entityLocalId = archetypes[destArchetype].addEntity(entityId, components);
            entities[entityId] = std::make_pair(destArchetype, entityLocalId);
        }

        std::vector<Archetype*>
        getArchetypesWith(const std::set<int>& include,
                          const std::set<int>& exclude);

    private:
        template <typename ComponentType, typename ...ComponentTypes>
        void initComponents(ComponentType &component, ComponentTypes &...components)
        {
            component.onCreation();
            if constexpr (sizeof...(ComponentTypes) != 0)
                initComponents(components...);
        }

        int addEntityType(const std::set<int>&& signature)
        {
            int  typeId          = getTypeId(signature);
            archetypes.insert({typeId, Archetype(signature, typeId)});
            std::unordered_map<int, int> browsedArchetypes;
            for (const int& componentTypeId : signature)
            {
                archetypeIdsByComponentType[componentTypeId][typeId] = typeId;
            }
            for (const int& componentTypeId : signature)
            {
                for (auto& pair : archetypeIdsByComponentType[componentTypeId])
                {
                    if (browsedArchetypes.find(pair.second) == browsedArchetypes.end())
                    {
                        Archetype& archetype = archetypes[pair.second];
                        if (std::includes(
                            archetype.signature.begin(), archetype.signature.end(),
                            signature.begin(), signature.end()))
                            archetypes[typeId].superArchetypes[pair.first] = &archetype;
                        else if (std::includes(
                            signature.begin(), signature.end(),
                            archetype.signature.begin(), archetype.signature.end()))
                            archetypes[pair.first].superArchetypes[typeId] = &archetypes[typeId];

                        browsedArchetypes[pair.second];
                    }
                }
            }
            return typeId;
        }

        bool entityTypeExists(const std::set<int>& signature)
        {
            return typeIds.find(signature) != typeIds.end();
        }

        int getTypeId(const std::set<int>& signature)
        {
            if (typeIds.find(signature) == typeIds.end())
            {
                typeIds.insert({signature, entityTypeCount++});
            }

            return typeIds[signature];
        }

        void transferEntity(Archetype& dest, Archetype& src, int& entityId);

        std::map<std::set<int>, int>    typeIds;
        int                                entityTypeCount = 0;
        std::unordered_map<int, std::unordered_map<int, int>> archetypeIdsByComponentType;
        std::unordered_map<int, Archetype> archetypes;
        NameAllocator entityIdAllocator = NameAllocator(10);
        std::unordered_map<int, std::pair<int, int>> entities;
    };
}  // namespace Gen

#endif