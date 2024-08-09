#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "GameEngine/Util/RTTI.h"
#include "Group.h"
#include <vector>
#include <unordered_map>

namespace Gen
{
    class EntityManager
    {
    public:
        template <typename ComponentType, typename... ComponentTypes>
        static int addEntity(ComponentType&& component,
                             ComponentTypes&&... components)
        {
            return groups[activeGroup].addEntity(component, components...);
        }

        static void removeEntity(int& entityId)
        {
            groups[activeGroup].removeEntity(entityId);
        }

        template <typename ComponentType>
        static bool hasComponent(int& entityId)
        {
            return groups[activeGroup].hasComponent<ComponentType>(entityId);
        }

        template <typename ComponentType>
        static ComponentType* getComponent(int& entityId)
        {
            return groups[activeGroup].getComponent<ComponentType>(entityId);
        }

        template <typename ComponentType, typename... ComponentTypes>
        static void addComponents(int& entityId, ComponentType&& component,
                                  ComponentTypes&&... components)
        {
            groups[activeGroup].addComponents(entityId, component,
                                              components...);
        }

        template <typename ComponentType, typename... ComponentTypes>
        static void removeComponents(int& entityId)
        {
            groups[activeGroup]
                .removeComponents<ComponentType, ComponentTypes...>(entityId);
        }

        static std::vector<Archetype*>
        getArchetypesWith(const std::vector<int>&& include,
                          const std::vector<int>&& exclude = {})
        {
            return groups[activeGroup].getArchetypesWith(include, exclude);
        }

        static void setActiveGroup(int& group)
        {
            activeGroup = group;
        }

    private:
        static int                            activeGroup;
        static std::unordered_map<int, Group> groups;
    };
}  // namespace Gen

#endif
