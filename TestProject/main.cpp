#include "GameEngine/util/Tree.h"
#include "GameEngine/util/Logger.h"
#include "GameEngine/System/Core.h"

int main()
{
	Gen::Tree<int> tree;
	tree.addNode(1, 2, 3);
	tree.addNode(1, 3, 4);
	tree.addNode(6, 4, 3);
	tree.addNode(6, 4, 3);
	Gen::BasicLoggingSetup();
	Gen::Logger logger{"Main"};
	logger.debug("Hello world!");
	logger.info("Hello world!");
	logger.warning("Hello world!");
	logger.error("Hello world!");
	logger.critical("Hello world!");
	logger.debug("Hello world!");

	Gen::Core::getInstance()->init();
	Gen::Core::getInstance()->run();
}