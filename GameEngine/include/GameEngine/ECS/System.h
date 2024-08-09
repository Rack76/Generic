#ifndef SYSTEM_H
#define SYSTEM_H

#include <functional>
#include <unordered_map>

#include "GameEngine/Util/RTTI.h"
#include "EntityManager.h"

namespace Gen
{
    class System
    {
    public:
        void runEntityProcs()
        {
            for (auto pair : entityProcs)
            {
                pair.second();
            }
        }

        void addEntityProc(int procName, std::function<void(Entity &&)> f,
                           std::vector<int> included,
                           std::vector<int> excluded = {})
        {
            entityProcs.insert({procName, [f, included, excluded]() 
                {
                std::vector<Archetype*> archetypes =
                EntityManager::getArchetypesWith(std::move(included),
                                                 std::move(excluded));
                for (auto archetype : archetypes)
                {
                    archetype->iterateEntities(f);
                }
                }});
        }

    private:
        std::unordered_map<int, std::function<void()>> entityProcs;
    };
}  // namespace Gen

#endif