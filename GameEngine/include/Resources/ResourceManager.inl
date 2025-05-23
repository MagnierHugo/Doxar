#pragma once

template <typename ResourceType> std::shared_ptr<ResourceType> ResourceManager::Load(const std::string& path) {

	const std::string& resourceTypeAsStr = typeid(ResourceType).name();

	if (resources[resourceTypeAsStr].contains(path)) {

		return std::static_pointer_cast<ResourceType>(resources[resourceTypeAsStr][path]);
	}

	std::shared_ptr<ResourceType> newResource = std::make_shared<ResourceType>();
	std::static_pointer_cast<Resource>(newResource)->Load(path);
	
	resources[resourceTypeAsStr][path] = newResource;

	return newResource;
}
