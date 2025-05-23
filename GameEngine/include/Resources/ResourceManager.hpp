#pragma once

#include <memory>
#include <string>
#include <unordered_map>


#include "Resources/Resource.hpp"

class ResourceManager
{
public:
	template<typename ResourceType> static std::shared_ptr<ResourceType> Load(const std::string& path);
	static void Init() { }
	static void Shutdown();
private:
	static std::unordered_map<std::string, std::unordered_map<std::string, std::shared_ptr<Resource>>> resources;
};

#include "Resources/ResourceManager.inl"