#ifndef RTTI_H
#define RTTI_H

namespace Gen
{
	class RTTI
	{
	public:
		template <typename T>
		static int typeId()
		{
			static int TypeId = typeIdCount++;
			return TypeId;
		}

	private:
		static int typeIdCount;
	};
}

#endif 