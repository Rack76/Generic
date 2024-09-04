#include "GameEngine/action/Actions.h"
#include <string>
#include "GameEngine/ECS/EntityManager.h"
#include "GameEngine/component/Camera.h"
#include "GameEngine/component/Transform.h"
#include "GameEngine/Util/Util.h"
#include "glm/vec3.hpp"

namespace Action
{
	void moveCameraLeft(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(LEFT);
	}

	void moveCameraRight(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(RIGHT);
	}

	void moveCameraUp(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(UP);
	}

	void moveCameraDown(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(DOWN);
	}

	void moveCameraForward(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(FRONT);
	}

	void moveCameraBackward(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		camera->setVelocity(BACK);
	}

	void stopCameraLeft(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingLeftward = (glm::dot(transform->orientation[0], camera->getVelocity()) > 0.0f);
		if (isTranslatingLeftward)
		{
			camera->setVelocity(NONE);
		}
	}

	void stopCameraRight(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingRightward = (glm::dot(-transform->orientation[0], camera->getVelocity()) > 0.0f);
		if (isTranslatingRightward)
		{
			camera->setVelocity(NONE);
		}
	}

	void stopCameraUp(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingUpward = (glm::dot(-transform->orientation[1], camera->getVelocity()) > 0.0f);
		if (isTranslatingUpward)
		{
			camera->setVelocity(NONE);
		}
	}

	void stopCameraDown(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingDownward = (glm::dot(transform->orientation[1], camera->getVelocity()) > 0.0f);
		if (isTranslatingDownward)
		{
			camera->setVelocity(NONE);
		}
	}

	void stopCameraForward(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingForward = (glm::dot(transform->orientation[2], camera->getVelocity()) > 0.0f);
		if (isTranslatingForward)
		{
			camera->setVelocity(NONE);
		}
	}

	void stopCameraBackward(std::any data, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		bool isTranslatingBackward = (glm::dot(-transform->orientation[2], camera->getVelocity()) > 0.0f);
		if (isTranslatingBackward)
		{
			camera->setVelocity(NONE);
		}
	}

	void rotateDebugCamera(std::any tuple, int entityId)
	{
		Gen::assertNoAbort(Gen::EntityManager::hasComponent<Gen::Camera>(entityId), "no camera component attached to entity " + std::to_string(entityId));
		Gen::Camera* camera = Gen::EntityManager::getComponent<Gen::Camera>(entityId);
		Gen::Transform* transform = Gen::EntityManager::getComponent<Gen::Transform>(entityId);
		std::tuple<double, double, float> data = std::any_cast<std::tuple<double, double, float>>(tuple);
		float xMotion = std::get<0>(data);
		float yMotion = std::get<1>(data);
		float dt = std::get<2>(data);
		xMotion *= 0.001;
		yMotion *= 0.001;
		camera->rotateCamera(xMotion, yMotion, dt);
	}
}