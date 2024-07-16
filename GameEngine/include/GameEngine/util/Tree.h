#ifndef TREE_H
#define TREE_H

#include <functional>
#include <type_traits>
#include <unordered_map>
#include <vector>
#include <utility>

namespace Gen
{
	template <typename ContainerType>
	class Tree
	{
	public:

		template <typename ElementType, typename ...Integers>
		void addNode(ElementType&& element, Integers &&...keys)
		{
			if constexpr (std::is_same<ContainerType, ElementType>::value)
			{
				addNodeRecurse(std::function<void(ContainerType&, ElementType&&)>(
					[](ContainerType& container, ElementType&& _element) {container = _element; }
				),
					std::move(element),
					std::move(keys)...);
			}
			else if constexpr (std::is_same<ContainerType, std::vector<ElementType>>::value)
			{
				addNodeRecurse(std::function<void(ContainerType&, ElementType&&)>(
					[](ContainerType& container, ElementType&& _element) {container.push_back(_element); }
				),
					std::move(element),
					std::move(keys)...);
			}
		}

	private:
		template <typename ElementType, typename Integer, typename ...Integers>
		void addNodeRecurse(
			std::function<void(ContainerType& container, ElementType&& element)>&& insertProc,
			ElementType&& element,
			Integer&& key,
			Integers &&...keys)
		{
			if constexpr (sizeof...(Integers) == 0)
			{
				insertProc(edges[key].container, std::move(element));
			}
			else
				edges[key].addNodeRecurse(
					std::move(insertProc),
					std::move(element),
					std::move(keys)...);
		}

		ContainerType container;
		std::unordered_map<int, Tree<ContainerType>> edges;
	};
}

#endif