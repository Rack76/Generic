#include "Generic/ECS/ComponentManager.h"

int Generic::ComponentManager::componentTypeIDCount = 0;;
std::unordered_map<int, std::unique_ptr<Generic::PoolAllocator>> Generic::ComponentManager::poolAllocators;
std::unordered_map<int, int> Generic::ComponentManager::componentTypeSizes;
std::unordered_map<int, std::string> Generic::ComponentManager::componentTypeNames;