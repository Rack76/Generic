#include "GameEngine/System/Renderer.h"
#include "GameEngine/System/Input.h"
#include "GameEngine/Core/Window.h"
#include "GameEngine/Core/Core.h"
#include "GameEngine/Core/File.h"
#include "GameEngine/Util/AssetManager.h"
#include "GameEngine/ECS/EntityManager.h"
#include "GameEngine/component/Mesh.h"
#include "GameEngine/component/Camera.h"
#include "GameEngine/component/PlayerInput.h"
#include "GameEngine/component/Transform.h"
#include "GameEngine/component/Texture.h"
#include "GameEngine/component/RigidBody.h"
#include "GameEngine/enum/ActionEnum.h"
#include "GameEngine/action/ActionManager.h"
#include "GameEngine/action/Actions.h"
#include <iostream>

namespace Gen
{
	void Core::init()
	{

		window = Window::getInstance();
		input = Input::getInstance();
		renderer = Renderer::getInstance();

		input->init();
		renderer->init();

		File::setWorkingDirectory("../../../../GameEngine/asset");

		ActionManager::addAction(MOVE_CAMERA_LEFT, Action::moveCameraLeft);
		ActionManager::addAction(MOVE_CAMERA_RIGHT, Action::moveCameraRight);
		ActionManager::addAction(MOVE_CAMERA_FORWARD, Action::moveCameraForward);
		ActionManager::addAction(MOVE_CAMERA_BACKWARD, Action::moveCameraBackward);
		ActionManager::addAction(MOVE_CAMERA_UP, Action::moveCameraUp);
		ActionManager::addAction(MOVE_CAMERA_DOWN, Action::moveCameraDown);

		ActionManager::addAction(STOP_CAMERA_LEFT, Action::stopCameraLeft);
		ActionManager::addAction(STOP_CAMERA_RIGHT, Action::stopCameraRight);
		ActionManager::addAction(STOP_CAMERA_FORWARD, Action::stopCameraForward);
		ActionManager::addAction(STOP_CAMERA_BACKWARD, Action::stopCameraBackward);
		ActionManager::addAction(STOP_CAMERA_UP, Action::stopCameraUp);
		ActionManager::addAction(STOP_CAMERA_DOWN, Action::stopCameraDown);

		ActionManager::addAction(ROTATE_DEBUG_CAMERA, Action::rotateDebugCamera);

		AssetManager::Model::loadModel("circle", "model/CercleBasic.dae");
		AssetManager::Model::loadModel("cube", "model/RectangleBasic.dae");
		AssetManager::Model::loadModel("piller", "model/Piller.dae");
		AssetManager::Shader::loadShaderProgram("basic", "shader/basic.vert", "shader/basic.frag");
		AssetManager::Shader::loadShaderProgram("basicTexture", "shader/basic.vert", "shader/basicTexture.frag");
		AssetManager::Texture::loadTexture("image1", "texture/texture_01.png");

		ComponentManager::registerComponentType<Mesh>();
		ComponentManager::registerComponentType<PlayerInput>();
		ComponentManager::registerComponentType<Camera>();
		ComponentManager::registerComponentType<Transform>();
		ComponentManager::registerComponentType<Texture>();
		ComponentManager::registerComponentType<RigidBody>();

		int entity = EntityManager::addEntity(Mesh{});
		EntityManager::getComponent<Mesh>(entity)->setMesh("cube");
		EntityManager::getComponent<Mesh>(entity)->setShaderProgram("basicTexture");

		EntityManager::addComponents(entity, Texture{});
		EntityManager::getComponent<Texture>(entity)->setTexture("image1");

		EntityManager::getComponent<Transform>(entity)->translation.x = -3;

		int piller = EntityManager::addEntity(Mesh{});
		EntityManager::getComponent<Mesh>(piller)->setMesh("piller");
		EntityManager::getComponent<Mesh>(piller)->setShaderProgram("basicTexture");

		EntityManager::addComponents(piller, Texture{});
		EntityManager::getComponent<Texture>(piller)->setTexture("image1");

		int circle = EntityManager::addEntity(Mesh{});
		EntityManager::getComponent<Mesh>(circle)->setMesh("circle");
		EntityManager::getComponent<Mesh>(circle)->setShaderProgram("basicTexture");

		EntityManager::addComponents(circle, Texture{});
		EntityManager::getComponent<Texture>(circle)->setTexture("image1");

		EntityManager::getComponent<Transform>(circle)->translation.y = 4.5;

		int camera = EntityManager::addEntity(Camera{});
	}

	void Core::run()
	{
		static float dt = 0;
		while (!window->shouldClose())
		{
			clock.reset();
			input->run(dt);
			renderer->run(dt);
			window->update();
			dt = clock.getTime();
		}
	}

	Window* Core::window;
	Input* Core::input;
	Renderer* Core::renderer;
	Clock Core::clock;
}