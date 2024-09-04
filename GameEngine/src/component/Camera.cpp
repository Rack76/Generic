#include "GameEngine/component/Camera.h"

namespace Gen
{
	void Camera::onCreation()
	{
		Window* window = Window::getInstance();
		perspective = glm::perspective(0.5f, (float)window->getWidth() / (float)window->getHeight(), 0.5f, 50.0f);

		if (!EntityManager::hasComponent<Transform>(entityId))
			EntityManager::addComponents(entityId, Transform{});

		Transform* transform = EntityManager::getComponent<Transform>(entityId);
		transform->translation = glm::vec3(0.0f, 0.0f, -10);

		if (!EntityManager::hasComponent<PlayerInput>(entityId))
			EntityManager::addComponents(entityId, PlayerInput{});

		PlayerInput* playerInput = EntityManager::getComponent<PlayerInput>(entityId);
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

	void Camera::rotateCamera(float& _xAngle, float& _yAngle, float dt)
	{
		xAngle -= _xAngle;
		yAngle -= _yAngle;
		if (yAngle > glm::radians(88.0f))
			yAngle = glm::radians(88.0f);
		if (yAngle < -glm::radians(88.0f))
			yAngle = -glm::radians(88.0f);
		Transform* transform = EntityManager::getComponent<Transform>(entityId);
		transform->identity().rotateYWorld(xAngle).rotateXWorld(yAngle);
		update(dt);
	}

	glm::vec3 Camera::getVelocity()
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

	void Camera::update(float dt)
	{
		Transform* transform = EntityManager::getComponent<Transform>(entityId);
		transform->translation = transform->translation + getVelocity() * 10.0f * dt;
		setUniform();
	}

	Camera* Camera::getActiveCamera()
	{
		//assertNoAbort(activeCamera != nullptr, "no active camera, bind a camera or creat a new one");
		return EntityManager::getComponent<Camera>(activeCamera);
	}

	void Camera::setUniform()
	{
		assertNoAbort(EntityManager::hasComponent<Transform>(entityId), "no transform component on entity " + std::to_string(entityId));
		Transform* transform = EntityManager::getComponent<Transform>(entityId);
		glm::mat4 view = glm::mat4(transform->orientation);
		view = glm::transpose(view);
		view[3] = glm::vec4(transform->translation * transform->orientation, 1.0);
		view = glm::transpose(view);
		AssetManager::Shader::setUniform("view", &view[0][0]);
	}
}

int Gen::Camera::activeCamera = -1;