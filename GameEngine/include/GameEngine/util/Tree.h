#ifndef TREE_H
#define TREE_H

#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>

namespace Gen
{
    template <typename ContainerType>
    class Tree
    {
    public:
        Tree()
        {
            if constexpr (std::is_same<ContainerType, int>::value)
                container = -1;
        }

        template <typename ElementType>
        void addNode(const ElementType& element, std::vector<int> keys)
        {
            if constexpr ( std::is_same<ContainerType, std::vector<int>>::value )
            {
                addNodeRecurse(
                    std::function<void(ContainerType&, const ElementType&)>(
                        [](ContainerType& container,
                            const ElementType& _element) {
                                container.push_back(_element);
                        }),
                    element, keys);
            }
            else if constexpr (std::is_same<ContainerType, ElementType>::value)
            {
                addNodeRecurse(
                    std::function<void(ContainerType&, const ElementType&)>(
                        [](ContainerType&     container,
                           const ElementType& _element) {
                            container = _element;
                        }),
                    element, keys);
            }
        }

        void getElement(ContainerType& element, std::vector<int> &keys, int index = 0)
        {
            if (index == keys.size())
            {
                element = container;
            }
            else
            {
                int key = keys[index];
                edges[key].getElement(element, keys, index + 1);
            }
        }

        void getElements(ContainerType& elements, std::vector<int> &keys,
                         std::unordered_map<int, int>& exclude, int index = 0)
        {
            if (index == keys.size())
            {
                if constexpr (std::is_same<ContainerType,
                                           std::vector<int>>::value)
                {
                    std::function<void(ContainerType&)> insertProc =
                        [&elements, &exclude](ContainerType& container) {
                            for (int& element : container)
                            {
                                if (std::find(elements.begin(), elements.end(), element) == elements.end())
                                       elements.push_back(element);
                            }
                        };
                    getElementsRecurse(insertProc, exclude);
                }
            }
            else
            {
                int key = keys[index];
                edges[key].getElements(elements, keys, exclude, index + 1);
            }
        }

    private:
        template <typename ElementType>
        void addNodeRecurse(
            const std::function<void(ContainerType&     container,
                                     const ElementType& element)>& insertProc,
            const ElementType& element, std::vector<int>& keys, int index = 0)
        {
            if (index == keys.size())
            {
                insertProc(container, element);
            }
            else
            {
                int key = keys[index];
                edges[key].addNodeRecurse(insertProc, element, keys, index + 1);
            }
        }

        void getElementsRecurse(std::function<void(ContainerType&)>& insertProc,
                                std::unordered_map<int, int>         exclude)
        {
            insertProc(container);
            for (auto& pair : edges)
            {
                if (exclude.find(pair.first) == exclude.end())
                {
                    pair.second.getElementsRecurse(insertProc, exclude);
                    exclude.insert({pair.first, pair.first});
                }
            }
        }

        ContainerType                                container;
        std::unordered_map<int, Tree<ContainerType>> edges;
    };
}  // namespace Gen

#endif