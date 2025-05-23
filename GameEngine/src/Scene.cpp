#include "Scene.hpp"
#include "GameObject.hpp"
#include "Components/Transform.hpp"
#include "Engine.hpp"


Scene::Scene(const std::string& name_, const bool active_) {
	active = active_;
	name = name_;
}

void Scene::Awake() {
	onAwake();
}

void Scene::Update() {

	Awake();

	for (GameObject* gameObject : gameObjects) {

		if (!gameObject->enabled) { continue; }

		gameObject->Update();
	}

	DeleteGameObjects();
}

void Scene::Render() {

	for (GameObject* gameObject : gameObjects) {

		if (!gameObject->enabled) { continue; }

		gameObject->Render();
#ifdef DEBUG
		if (Engine::debugModeEnabled) {

			gameObject->OnDebug();
		}
#endif
	}
}

void Scene::OnDestroy() {

	for (GameObject* gameObject : gameObjects) {

		delete gameObject;
		gameObject = nullptr;
	}

	gameObjects.clear();
}

#ifdef DEBUG
GameObject* Scene::AddGameObject(const std::string& name, const std::string& trackLifeCycleAs, const bool startActive) {

	GameObject* newGameObjectPtr = new GameObject(this, name, startActive);

	newGameObjectPtr->SetTransform(newGameObjectPtr->AddComponent<Transform>().get());

	gameObjects.push_back(newGameObjectPtr);

	if (trackLifeCycleAs != "") {
		newGameObjectPtr->trackedAs = trackLifeCycleAs;
		std::cout << "Created " << trackLifeCycleAs << "\n";
	}

	return newGameObjectPtr;
}
#else
GameObject* Scene::AddGameObject(const std::string& name, const bool startActive) {

	GameObject* newGameObjectPtr = new GameObject(this, name, startActive);

	newGameObjectPtr->SetTransform(newGameObjectPtr->AddComponent<Transform>().get());

	gameObjects.push_back(newGameObjectPtr);

	return newGameObjectPtr;
}
#endif

void Scene::QueueAwake(std::function<void(void)> componentAwakeMethod) {
	onAwake += componentAwakeMethod;
}

nlohmann::json Scene::Serialize() const {
	nlohmann::json gameObjectsJson = nlohmann::json::array();
	for (GameObject* gameObject : gameObjects) {
		gameObjectsJson.push_back(gameObject->Serialize());
	}

	return {
		{ "SceneName", name },
		{ "Loaded", active },
		{ "GameObjects", gameObjectsJson }
	};
}
