#ifndef RIGID_BODY_H
#define RIGID_BODY_H

#include <functional>
#include <unordered_map>
#include "glm/glm.hpp"
#include "GameEngine/Util/NameAllocator.h"

namespace Gen
{
	class RigidBody : public Component
	{
	public:
		int addForceGenerator(std::function<glm::vec3(RigidBody*)>& f)
		{
			int name = forceGeneratorsAllocator.getName();
			forceGenerators[name] = f;
			return name;
		}

		void removeForceGenerator(int name)
		{
			forceGenerators.erase(name);
			forceGeneratorsAllocator.returnName(name);
		}

		void addForces()
		{
			for (auto pair : forceGenerators)
			{
				auto forceGenerator = pair.second;
				netForce += forceGenerator(this);
			}
		}

		void clearForces()
		{
			netForce = glm::vec3(0.0f, 0.0f, 0.0f);
		}

	private:
		std::unordered_map<int, std::function<glm::vec3(RigidBody*)>> forceGenerators;
		glm::vec3 netForce = glm::vec3(0.0f, 0.0f, 0.0f);
		NameAllocator forceGeneratorsAllocator;
	};
}

#endif