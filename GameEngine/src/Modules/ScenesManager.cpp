#include <fstream>

#include "Engine.hpp"

#include "Modules/ScenesManager.hpp"
#include "Modules/RenderModule.hpp"

#include "Scene.hpp"

#include "Utilities/JSON.hpp"
#include "Components/Transform.hpp"


#define StripQuotes(s) ((s).front() == '"' && (s).back() == '"' ? (s).substr(1, (s).size() - 2) : (s))


ScenesManager* ScenesManager::instance = nullptr;
Scene* ScenesManager::persistentScene = nullptr;

ScenesManager::ScenesManager() {

	if (instance != nullptr) { throw std::runtime_error("FFS"); }

	instance = this;
	persistentScene = new Scene("PersistentScene", true);
}


void ScenesManager::Init() {
	renderModule = Engine::GetModule<RenderModule>();
}

void ScenesManager::Update() {

	initScenes();

	persistentScene->Update();

	for (auto& [__, scene] : scenes) {

		if (!scene->active) { continue; }

		scene->Update();
	}
}

void ScenesManager::Render() {
	if (renderModule->mainCamera == nullptr) { return; }

	persistentScene->Render();

	for (auto& [__, scene] : scenes) {

		if (!scene->active) { continue; }

		scene->Render();
	}
}


void ScenesManager::Shutdown() {

	std::cout << "Shutdown ScenesManager" << std::endl;
	persistentScene->OnDestroy();
	delete persistentScene;
	persistentScene = nullptr;

	for (auto& [__, scene] : scenes) {
		scene->OnDestroy();
		delete scene;
		scene = nullptr;
	}
	
	scenes.clear();
}

void ScenesManager::AddScene(const std::string& name, bool doStartActive) {
#if DEBUG
	if (instance->scenes.count(name) > 0) {

		std::cerr << "There is already a scene named: " << name << std::endl;
		throw std::runtime_error("^^");
	}
#endif
	Scene* newScene = new Scene(name, doStartActive);
	instance->initScenes += [=]() { newScene->Init(); };
	instance->scenes[name] = newScene;
}

void ScenesManager::RemoveScene(const std::string& name) {

	delete instance->scenes[name];
	instance->scenes.erase(name);
}

void ScenesManager::LoadScene(const std::string& name, bool additive) {

	if (!additive) {
		//UnloadAllScenes();
	}

	instance->scenes[name]->active = true;
}

void ScenesManager::UnloadAllScenes() {

	for (auto& [__, scene] : scenes) {
		scene->active = false;
	}
}

Scene* ScenesManager::GetScene(const std::string& name) {
#if DEBUG
	if (instance->scenes.count(name) == 0) {

		std::cerr << "There is no scene named: " << name << std::endl;
		return nullptr;
	}
#endif
	return instance->scenes[name];
}

void ScenesManager::ForceInitScenes() {
	initScenes.Invoke();
}

bool ScenesManager::ParseScenes() { // Lowkey loathe all these string references

	std::ifstream file("Assets/Scenes.json");
	if (!file) {
		std::cerr << "Failed to open scenes.json" << std::endl;
		return false;
	}
	
	nlohmann::json scenesJson;
	file >> scenesJson;
	
	for (const auto& sceneJson : scenesJson["Scenes"]) {

		std::string sceneName = StripQuotes(sceneJson["SceneName"].dump());
		std::cout << "Loading: " << sceneName << std::endl;
		Scene* newScene = new Scene(sceneName, sceneJson["Loaded"]);
		scenes[sceneName] = newScene;
		for (const auto& gameObjectJson : sceneJson["GameObjects"]) {

			std::string gameObjectName = StripQuotes(gameObjectJson["GameObjectName"].dump());
			std::cout << "Loading: " << gameObjectName << std::endl;
			GameObject* newGameObject = new GameObject(newScene, gameObjectName, gameObjectJson["Enabled"]);
			newScene->gameObjects.push_back(newGameObject);

			for (const auto& componentJson : gameObjectJson["Components"]) {
				
				std::string componentType = StripQuotes(componentJson["ComponentType"].dump());
				std::cout << "Loading: " << componentType << std::endl;
				auto it = Component::GetComponentBuilder().find(componentType);
				if (it == Component::GetComponentBuilder().end()) {
					std::cout << "Couldn t find builder for ComponentType: " << componentType << std::endl;
					return false;
				}
				auto componentPtr = it->second();
				std::shared_ptr<Component> asComponent = std::static_pointer_cast<Component>(componentPtr);

				asComponent->SetOwner(newGameObject);

				newGameObject->components.push_back(asComponent);

				newScene->QueueAwake([=]() { asComponent->Awake(); });

				componentPtr->Deserialize(componentJson);
			}
		}
	}

	std::cout << "Finished" << std::endl;
	return true;
}


bool ScenesManager::SaveScenes() {

	std::ofstream outFile("Assets/Scenes.json");
	if (!outFile) {
		std::cerr << "Failed to open Assets/Scenes.json for writing." << std::endl;
		return false;
	}

	nlohmann::json scenes_ = nlohmann::json::array();
	for (auto& [__, scene] : scenes) {
		scenes_.push_back(scene->Serialize());
	
	}
	nlohmann::json json = nlohmann::json::object();
	json["Scenes"] = scenes_;

	outFile << json.dump(4, ' ', false, nlohmann::json::error_handler_t::replace);
	std::cout << "Scene written to Assets/Scenes.json" << std::endl;

	return true;
}