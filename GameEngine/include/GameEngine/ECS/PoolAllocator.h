#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include "Component.h"
#include "GameEngine/Util/NameAllocator.h"
#include "GameEngine/Util/Util.h"

#include <memory>
#include <functional>
#include <unordered_map>
#include <vector>
#include <string>

namespace Gen
{
    class PoolAllocator
    {
    public:
        PoolAllocator() : maxBlockCount(0), componentTypeName(""), blockSize(0)
        {
        }

        PoolAllocator(int &maxBlockCount, const std::string &&componentTypeName,
                      const int &blockSize)
            : maxBlockCount(maxBlockCount),
              componentTypeName(componentTypeName),
              blockSize(blockSize)
        {
        }

        template <typename T>
        void setType()
        {
            assign = [](Component *dest, Component *src) {
                *reinterpret_cast<T *>(dest) = *reinterpret_cast<T *>(src);
            };
            assertNoAbort(std::is_base_of<Component, T>::value,
                          componentTypeName + " must inherit from Component");
            if constexpr (std::is_base_of<Component, T>::value)
            {
                construct = []() -> std::unique_ptr<Component> {
                    return std::make_unique<T>();
                };
            }
        }

        Component *getComponent(int entityTypeId, Component *component);
        void       returnComponent(int entityTypeId, Component *component);
        void       alloc();

    private:
        std::unordered_map<Component *, int>   componentIndices;
        std::unordered_map<int, int>           blocks;
        std::unordered_map<int, NameAllocator> componentAllocators;
        int                                    blockCount = 0;
        const int                              maxBlockCount;
        const int                              blockSize;
        const std::string                      componentTypeName;
        std::vector<std::vector<std::unique_ptr<Component>>> data;
        std::function<void(Component *, Component *)>        assign;
        std::function<std::unique_ptr<Component>()>          construct;
    };
}  // namespace Gen

#endif