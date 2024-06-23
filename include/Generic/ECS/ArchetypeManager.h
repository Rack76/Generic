#ifndef ARCHETYPE_MANAGER_H
#define ARCHETYPE_MANAGER_H

#include <iostream>
#include <unordered_map>
#include "Generic/Util/VLUI64.h"
#include "Generic/ECS/Archetype.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/ECS/ComponentManager.h"
#include "Generic/Util/RTTI.h"
#include <vector>
#include <string>

namespace Generic {

	class ArchetypeManager
	{
	public:
		static void removeEntity(int entityId);
        static Component* getComponent(const int &entityId, const int& componentTypeId);
        static void addEntityTypeName(const int& entityTypeId, std::string &&name);

        template <typename T, typename ...Types>
        static int addEntityType() {
            std::vector<int> signature = { GRTTI::_typeId<T>(), GRTTI::_typeId<Types>()... };
            return addEntityType(mergeSort(signature));
        }

        static int addEntityType(std::vector<int>&& signature)
        {
            int entityTypeId = GRTTI::typeId(std::move(signature));
            entityTypeNames[entityTypeId] = "";
            for (int i = 0; i < signature.size(); i++)
            {
                assertNoAbort([&entityTypeId]()->bool {
                    return ComponentManager::componentTypeNames.find(entityTypeId) == ComponentManager::componentTypeNames.end();
                    }, "ArchetypeManager :: addEntityType :: component type not found!");
                entityTypeNames[entityTypeId] += ComponentManager::componentTypeNames[signature[i]] + " ";
            }
            archetypes.insert({ entityTypeId, Archetype() });
            archetypes[entityTypeId].id = entityTypeId;
            archetypes[entityTypeId].signature = signature;
            enableArchetype(signature, entityTypeId);
            return entityTypeId;
        }

        static void enableArchetype(const std::vector<int> &signature, const int& id)
        {
            archetypeRecord.insert(signature, id);
            for (int componentTypeId : signature)
            {
                std::vector<int> subArchetype;
                for (int i = 0; i < signature.size(); i++)
                {
                    if (signature[i] == componentTypeId)
                        continue;
                    subArchetype.push_back(signature[i]);
                }
                superArchetypesArchive.insert(subArchetype, std::make_pair(componentTypeId, id));
            }
        }

        static void disableArchetype(const std::vector<int>& signature, const int& id)
        {
            archetypeRecord.remove(signature, id);
            for (int componentTypeId : signature)
            {
                std::vector<int> subArchetype;
                for (int i = 0; i < signature.size(); i++)
                {
                    if (signature[i] == componentTypeId)
                        continue;
                    subArchetype.push_back(signature[i]);
                }
                superArchetypesArchive.remove(subArchetype, id);
            }
            superArchetypesArchive.remove(signature, id);
        }

        template <typename T, typename ...Types>
        static void addComponents(const int& entityId, T &&component, Types &&..._components) {
            std::vector<int>&& signature = mergeSort({ GRTTI::_typeId<T>() , GRTTI::_typeId<Types>()... });
            int entityTypeId = entityLocations[entityId].first;
            std::vector<int> archetypeSignature = archetypes[entityTypeId].signature;
            std::vector<int> destArchetypeSignature = merge(archetypeSignature, signature);
            int destArchetype;
            archetypeRecord.getElement(destArchetypeSignature, destArchetype);
            if (destArchetype == -1)
            {
                addEntityType(std::move(destArchetypeSignature));
                archetypeRecord.getElement(destArchetypeSignature, destArchetype);
            }
            std::unordered_map<int, Component&&>&& components = { {GRTTI::_typeId<T>(), component}, {GRTTI::_typeId<Types>(), _components}... };
            archetypes[entityTypeId].transferEntity(entityTypeId, destArchetype, entityId, std::move(components));
        }

        template <typename T, typename ...Types>
        static void removeComponents(const int& entityId) {
            std::vector<int>&& excludeVec;// = mergeSort({ GRTTI::_typeId<T>() , GRTTI::_typeId<Types>()... });
            std::unordered_map<int, int> exclude;
            for (int componentTypeId : excludeVec)
            {
                exclude.insert({ componentTypeId, 0 });
            }
            int entityTypeId = entityLocations[entityId].first;
            std::vector<int> archetypeSignature = archetypes[entityTypeId].signature;
            int destArchetype;
            archetypeRecord.getElement(archetypeSignature, destArchetype, std::move(exclude));
            if (destArchetype == -1)
            {
                addEntityType(std::move(archetypeSignature));
                archetypeRecord.getElement(archetypeSignature, destArchetype, std::move(exclude));
            }
            archetypes[entityTypeId].transferEntity(entityTypeId, destArchetype, entityId);
        }

        template <typename T, typename ...Types>
        static bool entityTypeExists() {
            return entityTypeMasks.find(GRTTI::typeId<T, Types...>()) != entityTypeMasks.end();
        }

        static bool entityTypeExists(const int& entityTypeId);

        template <typename T, typename ...Types>
        static int addEntity(T&& arg, Types &&...args) {
            int entityId = entityIDsAllocator.getName();
            static int entityTypeId = addEntityType<T, Types...>();
            assertNoAbort([]()->bool {return archetypes.find(entityTypeId) != archetypes.end(); },
                "ArchetypeManager :: addEntity :: entity type not found");
            int entityLocalId = archetypes[entityTypeId].addEntityWithArgs(entityTypeId, std::move(arg), std::move(args)...);
            entityLocations[entityId] = { entityTypeId, entityLocalId };
            return entityId;
        }

        template <typename T, typename ...Types>
        static int addEntity() {
            int entityId = entityIDsAllocator.getName();
            static int entityTypeId = addEntityType<T, Types...>();
            assertNoAbort([]()->bool {return entityTypeExists(entityTypeId); },
                "ArchetypeManager :: addEntity :: entity type not found");
            int entityLocalId = archetypes[entityTypeId].addEntity(entityTypeId);
            entityLocations[entityId] = { entityTypeId, entityLocalId };
            return entityId;
        }

        static void getArchetypesWithComponents(const std::vector<int> &&include, std::unordered_map<int, int> &archetypes, const std::vector<int>&& exclude) {
            std::unordered_map<int, int> excludeComponentTypeIds;
            for (int componentTypeId : exclude)
            {
                excludeComponentTypeIds.insert({componentTypeId, 0});
            }
            auto signature = mergeSort(include);
            superArchetypesArchive.getElementsRecurse(signature, archetypes, excludeComponentTypeIds);
            int element;
            archetypeRecord.getElement(signature, element);
            archetypes.insert({ element, 0 });
        }

        static std::unordered_map<int, VLUI64> entityTypeMasks;
        static std::unordered_map<int, Archetype> archetypes;

    private:
        template <typename T, typename ...Types>
        static VLUI64& getEntityTypeMask() {
            static VLUI64 entityTypeMask = (VLUI64(GRTTI::_typeId<Types>()) | ... | VLUI64(GRTTI::_typeId<T>()));
            return entityTypeMask;
        }

        static constexpr int maxEntityCount = 20;
        static int entityTypeIDCount;
        static std::unordered_map<int, std::vector<int>> componentTypeIdsList;
        static NameAllocator typeIDsAllocator;
        static NameAllocator entityIDsAllocator;
        static std::unordered_map<int, std::pair<int, int>> entityLocations;
        static std::unordered_map<int, std::string> entityTypeNames;
        static SharedRecord superArchetypesArchive;
        static Record archetypeRecord;
	};
}

#endif 