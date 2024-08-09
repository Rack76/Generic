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
                std::vector<int>{RTTI::typeId<ComponentType>(),
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
            std::vector<int> signature =
                mergeSort(std::vector<int>{RTTI::typeId<ComponentTypes>()...});
            int              srcArchetype  = entities[entityId].first;
            std::vector<int> srcSignature  = archetypes[srcArchetype].signature;
            std::vector<int> destSignature = merge(srcSignature, signature);
            int              element;
            typeIdTree.getElement(element, destSignature);
            int destArchetype = element;
            if (element == -1)
                destArchetype = addEntityType(std::move(destSignature));

            transferEntity(archetypes[destArchetype], archetypes[srcArchetype],
                           entityId);
            int entityLocalId = entities[entityId].second;
            archetypes[destArchetype].addComponents(entityId, entityLocalId,
                                                    components...);
        }

        template <typename... ComponentTypes>
        void removeComponents(int& entityId)
        {
            std::vector<int> signature =
                mergeSort(std::vector<int>{RTTI::typeId<ComponentTypes>()...});
            int              srcArchetype = entities[entityId].first;
            std::vector<int> srcSignature = archetypes[srcArchetype].signature;
            std::vector<std::pair<int, int>> destSignatureInit;
            for (int& id : signature)
            {
                srcSignature.push_back(id);
            }
            for (int& id : srcSignature)
            {
                destSignatureInit.push_back(std::make_pair(id, id));
            }
            std::unordered_map<int, int> destSignatureMap =
                std::unordered_map<int, int>(destSignatureInit.begin(),
                                             destSignatureInit.end());
            for (int& id : signature)
            {
                destSignatureMap.erase(id);
            }
            std::vector<int> destSignature;
            for (auto it = destSignatureMap.begin();
                 it != destSignatureMap.end(); it++)
            {
                destSignature.push_back(it->first);
            }

            int element;
            typeIdTree.getElement(element, destSignature);
            int destArchetype = element;
            if (element == -1)
                destArchetype = addEntityType(std::move(destSignature));

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
        getArchetypesWith(const std::vector<int>& include,
                          const std::vector<int>& exclude);

    private:
        template <typename ComponentType, typename ...ComponentTypes>
        void initComponents(ComponentType &component, ComponentTypes &...components)
        {
            component.onCreation();
            if constexpr (sizeof...(ComponentTypes) != 0)
                initComponents(components...);
        }

        int addEntityType(const std::vector<int>&& signature)
        {
            auto sortedSignature = mergeSort(signature);
            int  typeId          = getTypeId(sortedSignature);
            typeIdTree.addNode(typeId, sortedSignature);
            archetypes.insert({typeId, Archetype(sortedSignature, typeId)});
            for (const int& id : sortedSignature)
            {
                std::vector<int> subEntityTypeSignature;
                for (int i = 0; i < sortedSignature.size(); i++)
                {
                    if (sortedSignature[i] != id)
                        subEntityTypeSignature.push_back(sortedSignature[i]);
                }
                superTypeIdsTree.addNode(typeId, subEntityTypeSignature);
            }
            return typeId;
        }

        int getTypeId(const std::vector<int>& signature)
        {
            if (typeIds.find(signature) == typeIds.end())
            {
                typeIds.insert({signature, entityTypeCount++});
            }

            return typeIds[signature];
        }

        void transferEntity(Archetype& dest, Archetype& src, int& entityId);

        std::map<std::vector<int>, int>    typeIds;
        Tree<int>                          typeIdTree;
        Tree<std::vector<int>>             superTypeIdsTree;
        int                                entityTypeCount = 0;
        std::unordered_map<int, Archetype> archetypes;
        NameAllocator entityIdAllocator = NameAllocator(10);
        std::unordered_map<int, std::pair<int, int>> entities;
    };
}  // namespace Gen

#endif