#ifndef TREE_H
#define TREE_H

#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <utility>

template <typename ContainerType>
class Tree
{
public:

	template <typename ElementType, typename ...Integers>
	void addNode( ElementType &&element,  Integers &&...keys)
	{
		if constexpr (std::is_same<ContainerType, ElementType>::value)
		{
			addNodeRecurse([](ContainerType& container, const ElementType&& element) {container = element; },
				std::move(element),
				std::move(keys)...);
		}
		else 
		{
			addNodeRecurse([](ContainerType& container, const ElementType&& element) {container.push_back(element); },
				std::move(element),
				std::move(keys)...);
		}
	}

	template <typename ElementType, typename Integer, typename ...Integers>
	void addNodeRecurse(
		 std::function<void(ContainerType& container, const ElementType &&element)> insertProc,
		 ElementType&& element, 
		 Integer &&key,
		 Integers &&...keys)
	{
		if constexpr (sizeof...(Integers) == 0)
		{
			insertProc(edges[key].container, std::move(element));
		}
		else
			edges[key].addNodeRecurse(
				std::move(insertProc),
				std::move(keys)...);
	}

private:
	ContainerType container;
	std::unordered_map<int, Tree<ContainerType>> edges;
};

#endif