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

		AssetManager::Model::loadModel("cube", "model/RectangleBasic.dae");
		AssetManager::Shader::loadShaderProgram("basic", "shader/basic.vert", "shader/basic.frag");
		AssetManager::Shader::loadShaderProgram("basicTexture", "shader/basic.vert", "shader/basicTexture.frag");
		AssetManager::Texture::loadTexture("image1", "texture/texture_01.png");

		ComponentManager::registerComponentType<Mesh>();
		ComponentManager::registerComponentType<PlayerInput>();
		ComponentManager::registerComponentType<Camera>();
		ComponentManager::registerComponentType<Transform>();
		ComponentManager::registerComponentType<Texture>();

		int entity = EntityManager::addEntity(Mesh{});
		EntityManager::getComponent<Mesh>(entity)->setMesh("cube");
		EntityManager::getComponent<Mesh>(entity)->setShaderProgram("basicTexture");

		EntityManager::addComponents(entity, Texture{});
		EntityManager::getComponent<Texture>(entity)->setTexture("image1");

		int camera = EntityManager::addEntity(Camera{});
	}

	void Core::run()
	{
		while (!window->shouldClose())
		{
			input->run();
			renderer->run();
			window->update();
		}
	}

	Window* Core::window;
	Input* Core::input;
	Renderer* Core::renderer;
}