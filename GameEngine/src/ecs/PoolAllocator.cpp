#include "GameEngine/ecs/PoolAllocator.h"
#include "GameEngine/util/Util.h"

namespace Gen
{
	Component* PoolAllocator::getComponent(int entityTypeId, Component *component)
	{
		assertNoAbort(blockCount < maxBlockCount, "PoolAllocator::getComponent :" 
			" maximum block count reached, you may increase block count for component type " + componentTypeName);

		if (blocks.find(entityTypeId) == blocks.end())
			blocks[entityTypeId] = blockCount++;

		int block = blocks[entityTypeId];
		int index = componentAllocators[block].getName();

		assertNoAbort(index < blockSize, "PoolAllocator::getComponent :"
			" cant allocate more components, you may increase block size for component type " + componentTypeName);
		Component *returnValue = data[block][index].get();
		assign(returnValue, component);
		componentIndices[returnValue] = index;
		return returnValue;
	}

	void PoolAllocator::returnComponent(int entityTypeId, Component *component)
	{
		assertNoAbort(blocks.find(entityTypeId) != blocks.end(), "PoolAllocator::returnComponent :"
			"no corresponding block found " + componentTypeName);

		int block = blocks[entityTypeId];

		assertNoAbort(componentIndices.find(component) != componentIndices.end(), "PoolAllocator::returnComponent :"
			"no corresponding component index found " + componentTypeName);

		int index = componentIndices[component];
		componentAllocators[block].returnName(index);
	}

	void PoolAllocator::alloc()
	{
		data.shrink_to_fit();
		data.resize(maxBlockCount);

		for (int a = 0; a < maxBlockCount; a++)
		{
			for (int b = 0; b < blockSize; b++)
			{
				data[a].push_back(construct());
			}
			componentAllocators.insert({ a, NameAllocator(blockSize) });
		}
	}
}