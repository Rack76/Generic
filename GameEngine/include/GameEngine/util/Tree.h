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
		template <typename ElementType>
		void addNode(const ElementType& element, std::vector<int> &&keys)
		{
			if constexpr (std::is_same<ContainerType, ElementType>::value)
			{
				addNodeRecurse(std::function<void(ContainerType&, const ElementType&)>(
					[](ContainerType& container, const ElementType& _element) {container = _element; }
				),
					element,
					keys);
			}
			else if constexpr (std::is_same<ContainerType, std::vector<ElementType>>::value)
			{
				addNodeRecurse(std::function<void(ContainerType&, const ElementType&)>(
					[](ContainerType& container, const ElementType& _element) {container.push_back(_element); }
				),
				    element,
					keys);
			}
		}

	private:
		template <typename ElementType>
		void addNodeRecurse(
			const std::function<void(ContainerType& container, const ElementType& element)>& insertProc,
			const ElementType& element,
			std::vector<int> &keys)
		{
			if (keys.size() == 0)
			{
				insertProc(container, element);
			}
			else
			{
				int key = keys[keys.size() - 1];
				keys.pop_back();
				edges[key].addNodeRecurse(
					insertProc,
					element,
					keys);
			}
		}

		ContainerType container;
		std::unordered_map<int, Tree<ContainerType>> edges;
	};
}

#endif