#ifndef COMPONENT_H
#define COMPONENT_H

namespace Gen
{

    class Component
    {
    public:
        virtual void onCreation() {

        }

        int entityId = -1;
    };
}  // namespace Gen

#endif