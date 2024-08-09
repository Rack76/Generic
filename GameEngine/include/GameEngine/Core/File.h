#ifndef FILE_H
#define FILE_H

#include "GameEngine/Util/Singleton.h"
#include "GameEngine/Util/NameAllocator.h"
#include <fstream>
#include <string>

namespace Gen
{
	enum Mode{
		READ = 1, 
		WRITE = 2
	};

	class File : public Singleton<File>
	{
	public:
		static void setWorkingDirectory(const char* path);

	private:
		static NameAllocator fileHandleAllocator;
		static std::unordered_map<int, std::fstream> files;
	};
}

#endif 
