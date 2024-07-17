#include "GameEngine/util/Tree.h"
#include "GameEngine/util/Logger.h"
#include "GameEngine/System/Core.h"
#include "GameEngine/service/EntityManager.h"

class Mesh{};
class Shader{};
class Texture{};

int main()
{
	Gen::Tree<int> tree;
	tree.addNode(1, std::vector<int>{2, 3});
	tree.addNode(1, std::vector<int>{3, 4});
	tree.addNode(6, std::vector<int>{4, 3});
	Gen::BasicLoggingSetup();
	Gen::Logger logger{"Main"};
	logger.debug("Hello world!");
	logger.info("Hello world!");
	logger.warning("Hello world!");
	logger.error("Hello world!");
	logger.critical("Hello world!");
	logger.debug("Hello world!");

	int typeId1 = Gen::EntityManager::addEntityType<Mesh>();
	int typeId2 = Gen::EntityManager::addEntityType<Mesh, Shader>();
	int typeId5 = Gen::EntityManager::addEntityType<Shader, Mesh>();
	int typeId3 = Gen::EntityManager::addEntityType<Shader, Texture>();
	int typeId4 = Gen::EntityManager::addEntityType<Texture>();

	Gen::Core::getInstance()->init();
	Gen::Core::getInstance()->run();
}