#ifndef POOL_ALLOCATOR_H
#define POOL_ALLOCATOR_H

#include <memory>
#include <functional>
#include <vector>
#include <unordered_map>
#include "Generic/ECS/Component.h"
#include "Generic/Util/NameAllocator.h"
#include "Generic/Util/Util.h"
#include "Generic/Util/RTTI.h"


namespace Generic {

	class PoolAllocator {
	public:

		virtual Component* getComponent(const int&) = 0;
		virtual Component* getComponentWithArg(const int&, Component &&c) = 0;
		virtual void alloc() = 0;
		virtual bool isEmpty() noexcept = 0;
		virtual void returnComponent(const int&, Component*) = 0;
		virtual void returnComponentWithCopy(const int&, Component*) = 0;
	};

	template <typename T>
	class PoolAllocatorTemplate : public PoolAllocator
	{
	public:
		virtual Component* getComponent(const int& entityTypeId) {
			assertNoAbort([&entityTypeId, this]()->bool {return blocks.find(entityTypeId) == blocks.end() && blocks.size() == blockCount;  },
				"PoolAllocator :: getComponent :: maximum block count reached"
				" you may increase block cout for component type " + GRTTI::typeName<T>() + " if required.");
			int block = blocks[entityTypeId];
			int index = componentIndicesAllocator[block].getName();
			assertNoAbort([&index]()->bool {return index < blockSize; }, "PoolAllocator :: getComponent :: component index out of bounds"
				" you may increase block size for component type" + GRTTI::typeName<T>() + " if required.");
			Component* c = reinterpret_cast<Component*>(&data[block][index]);
			reservedComponentIndices[block][c] = index;
			return c;
		}

		virtual Component* getComponentWithArg(const int& entityTypeId, Component &&arg) {
			assertNoAbort([&entityTypeId, this]()->bool {return blocks.find(entityTypeId) == blocks.end() && blocks.size() == blockCount; },
				"PoolAllocator :: getComponent :: maximum block count reached"
				" you may increase block cout for component type " + GRTTI::typeName<T>() + " if required.");
			int block = blocks[entityTypeId];
			int index = componentIndicesAllocator[block].getName();
			assertNoAbort([&index, this]()->bool {return index < blockSize; }, "PoolAllocator :: getComponent :: component index out of bounds"
				" you may increase block size for component type" + GRTTI::typeName<T>() + " if required.");
			data[block][index] = T(reinterpret_cast<T&&>(arg));
			Component* c = reinterpret_cast<Component*>(&data[block][index]);
			reservedComponentIndices[block][c] = index;
			return c;
		}

		virtual void alloc() {
			data.shrink_to_fit();
			data.reserve(blockCount * blockSize);
			for (int i = 0; i < blockCount; i++)
			{
				data.push_back(std::vector<T>());
				for (int a = 0; a < blockSize; a++)
				{
					data[i].push_back(T());
				}
				componentIndicesAllocator[i] = NameAllocator(blockSize);
			}
		}

		virtual bool isEmpty() noexcept {
			return data.size() == 0;
		}

		virtual void returnComponent(const int& entityTypeId, Component* c) {
			int componentBlock = blocks[entityTypeId];
			int componentIndex = reservedComponentIndices[componentBlock][c];
			int lastComponentIndex = componentIndicesAllocator[componentBlock].lastNameReserved();
			componentIndicesAllocator[componentBlock].returnName(componentIndex);
			reservedComponentIndices[componentBlock].erase(c);
		}

		virtual void returnComponentWithCopy(const int& entityTypeId, Component* c) {
			int componentBlock = blocks[entityTypeId];
			int componentIndex = reservedComponentIndices[componentBlock][c];
			int lastComponentIndex = componentIndicesAllocator[componentBlock].lastNameReserved();
			if (componentIndex != lastComponentIndex)
			{
				data[componentBlock][componentIndex] = data[componentBlock][lastComponentIndex];
				componentIndicesAllocator[componentBlock].returnName(lastComponentIndex);
				reservedComponentIndices[componentBlock].erase((--reservedComponentIndices[componentBlock].end())->first);
			}
			else
			{
				componentIndicesAllocator[componentBlock].returnName(componentIndex);
				reservedComponentIndices[componentBlock].erase(c);
			}
		}

	private:
		static const int blockCount = 3;
		static const int blockSize = 16;
		std::vector<std::vector<T>> data;
		std::unordered_map<int, int> blocks;
		std::unordered_map<int, NameAllocator> componentIndicesAllocator;
		std::unordered_map<int, std::unordered_map<Component*, int>> reservedComponentIndices;
	};
}

#endif 