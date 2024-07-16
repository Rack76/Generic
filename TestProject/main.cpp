#include "GameEngine/util/Tree.h"
#include "GameEngine/util/Logger.h"

int main()
{
	Gen::BasicLoggingSetup();
	Gen::Logger logger{"Main"};
	logger.debug("Hello world!");
	logger.info("Hello world!");
	logger.warning("Hello world!");
	logger.error("Hello world!");
	logger.critical("Hello world!");
	logger.debug("Hello world!");
}