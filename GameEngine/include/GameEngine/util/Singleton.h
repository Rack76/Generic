#ifndef SINGLETON_H
#define SINGETON_H

#include <memory>

template <typename T>
class Singleton
{
public:
	static T* getInstance()
	{
		static std::unique_ptr<T> instance = std::make_unique<T>();
		return instance;
	}

protected:
	Singleton<T>();
	Singleton<T>(const Singleton<T>&);
};

#endif 