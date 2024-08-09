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
		void onCreation()
		{
			Window* window = Window::getInstance();
			perspective = glm::perspective(0.5f, (float)window->getWidth() / (float)window->getHeight(), 0.5f, 50.0f);

			if (!EntityManager::hasComponent<Transform>(entityId))
				EntityManager::addComponents(entityId, Transform{});

			Transform *transform = EntityManager::getComponent<Transform>(entityId);
			transform->translation = glm::vec3(0.0f, 0.0f, -10);

			if (!EntityManager::hasComponent<PlayerInput>(entityId))
				EntityManager::addComponents(entityId, PlayerInput{});

			PlayerInput *playerInput = EntityManager::getComponent<PlayerInput>(entityId);
			playerInput->addAction(A_KEY_PRESS, Action::moveCameraLeft);
			playerInput->addAction(A_KEY_RELEASE, Action::stopCameraLeft);
			playerInput->addAction(D_KEY_PRESS, Action::moveCameraRight);
			playerInput->addAction(D_KEY_RELEASE, Action::stopCameraRight);
			playerInput->addAction(SPACE_KEY_PRESS, Action::moveCameraUp);
			playerInput->addAction(SPACE_KEY_RELEASE, Action::stopCameraUp);
			playerInput->addAction(LEFT_SHIFT_KEY_PRESS, Action::moveCameraDown);
			playerInput->addAction(LEFT_SHIFT_KEY_RELEASE, Action::stopCameraDown);
			playerInput->addAction(W_KEY_PRESS, Action::moveCameraForward);
			playerInput->addAction(W_KEY_RELEASE, Action::stopCameraForward);
			playerInput->addAction(S_KEY_PRESS, Action::moveCameraBackward);
			playerInput->addAction(S_KEY_RELEASE, Action::stopCameraBackward);

			playerInput->addAction(MOUSE_MOTION, Action::rotateDebugCamera);

			AssetManager::Shader::setUniform("perspective", &perspective[0][0]);

			if (activeCamera == -1)
				activeCamera = entityId;
		}

		void rotateCamera(float& _xAngle, float& _yAngle)
		{
			xAngle -= _xAngle;
			yAngle -= _yAngle;
			Transform* transform = EntityManager::getComponent<Transform>(entityId);
			transform->identity().rotateYWorld(xAngle).rotateXWorld(yAngle);
			update();
		}

		void setVelocity(int&& _velocityDir)
		{
			velocityDir = _velocityDir;
		}

		glm::vec3 getVelocity()
		{
			Transform* transform = EntityManager::getComponent<Transform>(entityId);
			switch (velocityDir)
			{
			case NONE:
				return glm::vec3(0.0f, 0.0f, 0.0f);
				break;
			case LEFT:
				return transform->orientation[0];
				break;
			case RIGHT:
				return -transform->orientation[0];
				break;
			case UP:
				return -glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case DOWN:
				return glm::vec3(0.0f, 1.0f, 0.0f);
				break;
			case FRONT:
				return transform->orientation[2];
				break;
			case BACK:
				return -transform->orientation[2];
				break;
			}
		}

		void update()
		{
			Transform* transform = EntityManager::getComponent<Transform>(entityId);
			transform->translation = transform->translation + getVelocity() * 0.005f;
			setUniform();
		}

		static Camera* getActiveCamera()
		{
			//assertNoAbort(activeCamera != nullptr, "no active camera, bind a camera or creat a new one");
			return EntityManager::getComponent<Camera>(activeCamera);
		}

		float xAngle = 0;
		float yAngle = 0;

	private:
		int velocityDir = NONE;

		glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);

		void setUniform()
		{
			assertNoAbort(EntityManager::hasComponent<Transform>(entityId), "no transform component on entity " + std::to_string(entityId));
			Transform* transform = EntityManager::getComponent<Transform>(entityId);
			glm::mat4 view = glm::mat4(transform->orientation);
			view = glm::transpose(view);
			view[3] = glm::vec4(transform->translation * transform->orientation, 1.0);
			view = glm::transpose(view);
			AssetManager::Shader::setUniform("view", &view[0][0]);
		}

		glm::mat4 perspective;

		static int activeCamera;
	};
}

#endif 