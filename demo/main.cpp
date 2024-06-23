#include "Generic/ECS/SystemManager.h"
#include "Generic/ECS/EntityManager.h"
#include "Generic/ECS/ComponentManager.h"
#include "Generic/Util/VLUI64.h"
#include "Generic/ECS/Archetype.h"
#include <iostream>
#include <cmath>

using namespace Generic;

class Renderer : public Component {public: Renderer() {}  Renderer(int &&a): var1(a) {} int var1 = 5; };
class glRenderer : public Component { public: int var1 = 2; };
class Physics : public Component { public: Physics() {} Physics(int&& a) : var1(a) {} int var1 = 3; };
class Shader : public Component{};
class Mesh{};

int main()
{
	ComponentManager::registerComponentType<Renderer>();
	ComponentManager::registerComponentType<glRenderer>();
	ComponentManager::registerComponentType<Physics>();
	ComponentManager::registerComponentType<Shader>();
	ComponentManager::alloc();
	int entity1 = EntityManager::addEntity<Physics, Renderer, glRenderer>();
	EntityManager::addEntity<Physics, glRenderer>();
    int	entity2 = EntityManager::addEntity<Renderer, glRenderer>();
	int entity3 = EntityManager::addEntity<Physics>();
	EntityManager::addEntity<glRenderer>();
	std::cout << EntityManager::getComponent<Renderer>(entity1)->var1 << std::endl;
	std::cout << EntityManager::getComponent<Renderer>(entity2)->var1 << std::endl;
	std::cout << EntityManager::getComponent<glRenderer>(entity2)->var1 << std::endl;
	std::cout << EntityManager::getComponent<Physics>(entity1)->var1 << std::endl;
	int	entity4 = EntityManager::addEntity(Renderer{7}, glRenderer{});
	int entity5 = EntityManager::addEntity(Physics{8});
	int entity6 = EntityManager::addEntity<Renderer, Physics>();
	std::cout << EntityManager::getComponent<Renderer>(entity4)->var1 << std::endl;
	std::cout << EntityManager::getComponent<glRenderer>(entity4)->var1 << std::endl;
	std::cout << EntityManager::getComponent<Physics>(entity5)->var1 << std::endl;
	EntityManager::removeEntity(entity4);
	std::unordered_map<int, int> archetypes1;
	EntityManager::getArchetypesWithComponents({GRTTI::_typeId<Physics>(), GRTTI::_typeId<Renderer>()}, archetypes1);
	std::unordered_map<int, int> archetypes2;
	EntityManager::getArchetypesWithComponents({ GRTTI::_typeId<Physics>() }, archetypes2);
	for (auto archetype : archetypes1)
	{

	}
	return 0;
}