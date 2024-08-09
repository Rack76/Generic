#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "glm/gtc/matrix_transform.hpp"
#include "GameEngine/ECS/Component.h"

namespace Gen
{
	class Transform : public Component
	{
	public:
		Transform()
		{

		}

		Transform& rotateXWorld(float &angle)
		{
			orientation = glm::rotate(glm::mat4(orientation), angle, glm::vec3(1.0f, 0.0f, 0.0f));
			return *this;
		}

		Transform& rotateYWorld(float& angle)
		{
			orientation = glm::rotate(glm::mat4(orientation), angle, glm::vec3(0.0f, 1.0f, 0.0f));
			return *this;
		}

		Transform& identity()
		{
			orientation = glm::mat4(1.0f);
			return *this;
		}

		glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::mat3 orientation = glm::mat3(1.0f);
	};
}

#endif 