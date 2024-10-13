#ifndef TERRAIN_MANAGER_H
#define TERRAIN_MANAGER_H

#include <string>

namespace Gen
{
	class TerrainManager
	{
	public:
		void genTerrainCollider(std::string&& terrain, float cellWidth, float cellHeight, float cellDepth);
	};
}

#endif
