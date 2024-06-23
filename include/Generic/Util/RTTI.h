#ifndef GENERIC_RTTI_H
#define GENERIC_RTTI_H

#include <typeinfo>
#include <string>
#include <map>
#include <cstdint>
#include "Generic/Util/VLUI64.h"

namespace Generic {
	struct GRTTI {
		template <typename T>
		static std::string typeName() {
			static std::string&& name = typeid(T).name();
			static std::string TypeName = name.erase(0, 6);
			return TypeName;
		}

		template <typename T>
		static int _typeId() {
			std::string a = typeName<T>();
			static int TypeId = _typeIDCount++;
			return TypeId;
		}

		static int initTypeId(const Generic::VLUI64&& mask) {
			if (typeIds.find(mask) == typeIds.end())
			{
				typeIds[mask] = typeIDCount++;
			}
			return typeIds[mask];
		}

		template <typename T, typename ...Types>
		static int typeId() {
			Generic::VLUI64&& typeMask = (VLUI64(GRTTI::_typeId<Types>()) | ... | VLUI64(GRTTI::_typeId<T>()));
			static int TypeId = initTypeId(std::move(typeMask));
			return TypeId;
		}

		static int typeId(std::vector<int> &&signature)
		{
			VLUI64 mask = VLUI64(std::move(signature));
			return initTypeId(std::move(mask));
		}

	private:
		static int typeIDCount;
		static std::map<Generic::VLUI64, int> typeIds;
		static int _typeIDCount;
		static const int TypeId;
	};
}

#endif 