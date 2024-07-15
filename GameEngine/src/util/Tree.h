#ifndef TREE_H
#define TREE_H

#include <functional>
#include <type_traits>
#include <unordered_map>

template <typename ContainerType>
class Tree
{
public:

	template <typename ElementType, typename ...Integers>
	void addNode(const ElementType &&element, const Integers &&keys...)
	{
		if constexpr (std::is_same<ElementType, ContainerType>::value)
			addNodeRecurse([](
				ContainerType& container, const ElementType&& element) {container = element; },
				std::move(element),
				std::move(keys)...);
		else if constexpr(std::is_same <std::vector<ElementType>, ContainerType>::value)
			addNodeRecurse([](
				ContainerType& container, const ElementType&& element) {container.push_back(element) },
				std::move(element),
				std::move(keys)...);
	}

	template <typename ElementType, typename Integer, typename ...Integers>
	void addNodeRecurse(
		const std::function<void(ContainerType& container, const ElementType &&element)> &&insertProc,
		const ElementType&& element, 
		const Integer&& key, 
		const Integers&& keys...)
	{
		if constexpr (sizeof...(Integers) == 0)
		{
			insertProc(edges[key].container, std::move(element));
		}
		else
			edges[key].addNodeRecurse(
				std::move(insertProc),
				std::move(element),
				std::move(key),
				std::move(keys)...);
	}

private:
	ContainerType container;
	std::unordered_map<int, Tree<ContainerType>> edges;
};

#endif