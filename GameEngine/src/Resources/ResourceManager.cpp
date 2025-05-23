#include "Resources/ResourceManager.hpp"

std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Resource>>> ResourceManager::resources;

void ResourceManager::Shutdown()
{
	resources.clear();
}
