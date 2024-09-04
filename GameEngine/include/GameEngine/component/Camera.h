#ifndef CAMERA_H
#define CAMERA_H

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "GameEngine/ECS/EntityManager.h"
#include "GameEngine/ECS/Component.h"
#include "GameEngine/Core/Window.h"
#include "GameEngine/component/Transform.h"
#include "GameEngine/component/PlayerInput.h"
#include "GameEngine/enum/InputEnum.h"
#include "GameEngine/action/Actions.h"
#include "GameEngine/Util/AssetManager.h"

enum VelocityDir {
	NONE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	FRONT,
	BACK
};

namespace Gen
{
	class Camera : public Component
	{
	public:
		void onCreation();

		void rotateCamera(float& _xAngle, float& _yAngle, float dt);

		void setVelocity(int&& _velocityDir)
		{
			velocityDir = _velocityDir;
		}

		glm::vec3 getVelocity();

		void update(float dt);

		static Camera* getActiveCamera();

		float xAngle = 0;
		float yAngle = 0;

	private:
		int velocityDir = NONE;

		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		void setUniform();

		glm::mat4 perspective;

		static int activeCamera;
	};
}

#endif 