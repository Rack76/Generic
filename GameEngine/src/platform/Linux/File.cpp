#include "GameEngine/Core/File.h"
#include <sstream>
#include "GameEngine/Util/Util.h"
#include <windows.h>
#include <iostream>

namespace Gen
{
	void File::setWorkingDirectory(const char* path)
	{
		assertNoAbort(chdir(path) != 0, "could not open file ");
	}

	NameAllocator File::fileHandleAllocator;
	std::unordered_map<int, std::fstream> File::files;
}