#ifndef SINGLETON_H
#define SINGLETON_H

#include <memory>

namespace Gen
{
	template <typename T>
	class Singleton
	{
	public:
		static T* getInstance()
		{
			static std::unique_ptr<T> instance = std::make_unique<T>();
			return instance.get();
		}

	protected:
		Singleton<T>();
		Singleton<T>(const Singleton<T>&);
	};
}

#endif 