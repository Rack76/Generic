#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "GameEngine/util/Tree.h"
#include "GameEngine/util/RTTI.h"
#include "GameEngine/util/Util.h"
#include <map>

namespace Gen
{
	class EntityManager
	{
	public:
		template <typename ComponentType, typename ...ComponentTypes>
		static void addEntity()
		{
			int entityTypeId = addEntityType<ComponentType, ComponentTypes...>();
		}

		template <typename ComponentType, typename ...ComponentTypes>
		static int addEntityType()
		{
			auto signature = mergeSort(std::vector<int>{ RTTI::typeId<ComponentType>(), RTTI::typeId<ComponentTypes>()...});
			int typeId = getTypeId(signature);
			typeIdTree.addNode(typeId, std::move(signature));
			return typeId;
		}

	private:
		static int getTypeId(std::vector<int> &signature)
		{
			if (typeIds.find(signature) == typeIds.end())
			{
				typeIds.insert({signature, entityTypeCount++});
			}

			return typeIds[signature];
		}


		static std::map<std::vector<int>, int> typeIds;
		static Tree<int> typeIdTree;
		static int entityTypeCount;
	};
}

#endif 