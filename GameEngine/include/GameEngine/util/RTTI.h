#ifndef RTTI_H
#define RTTI_H

#include <string>

namespace Gen
{
    class RTTI
    {
    public:
        template <typename T>
        static int typeId()
        {
            std::string name = typeid(T).name();
            static int TypeId = typeIdCount++;
            return TypeId;
        }

    private:
        static int typeIdCount;
    };
}  // namespace Gen

#endif
