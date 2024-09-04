#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/gtc/matrix_transform.hpp"
#include "GameEngine/ECS/Component.h"

namespace Gen
{
	class Transform : public Component
	{
	public:
		Transform& rotateXWorld(float& angle);

		Transform& rotateYWorld(float& angle);

		Transform& identity();

		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat3 orientation = glm::mat3(1.0f);
	};
}

#endif 