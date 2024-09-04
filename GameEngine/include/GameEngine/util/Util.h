#ifndef UTIL_H
#define UTIL_H

#include <vector>
#include <functional>
#include <string>

#include "GameEngine/Util/Logger.h"

namespace Gen
{
    std::vector<int> merge(std::vector<int>& elements1,
                           std::vector<int>& elements2);
    std::vector<int> mergeSort(const std::vector<int>& _elements);

    inline void assertNoAbort(bool condition, std::string&& msg)
    {
#ifndef NDEBUG
        if (!condition)
        {
            Logger::logError(msg);
        }
#endif
    }

    template <typename T>
    static std::string&& typeName()
    {
        static std::string&& name     = typeid(T).name();
        static std::string   TypeName = name.erase(0, 6);
        return std::move(TypeName);
    }
}  // namespace Gen

#endif