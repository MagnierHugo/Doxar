#pragma once

#include <string>

#include "Utilities/MulticastDelegate.hpp"
#include "Utilities/JSON.hpp"

#ifdef DEBUG
	#define AddGameObjectTracked(name, startActive, trackAs) AddGameObject(name, trackAs, startActive)
	#define AddGameObjectUntracked(name, startActive) AddGameObject(name, "", startActive)
#else
	#define AddGameObjectTracked(name, startActive, trackAs) AddGameObject(name, startActive)
	#define AddGameObjectUntracked(name, startActive) AddGameObject(name, startActive)
#endif


class Scene {
	friend class ScenesManager;
	friend class EditorWindows;
public:
	Scene(const std::string& name_, const bool active);
	bool active = true;
	std::string name;

	virtual void Init() { }

	void Awake();
	void Update();
	void Render();
	void OnDestroy();

#ifdef DEBUG
	class GameObject* AddGameObject(const std::string& name, const std::string& trackLifeCycleAs, const bool startActive = true);
#else
	class GameObject* AddGameObject(const std::string& name, const bool startActive = true);
#endif
	void QueueAwake(std::function<void(void)> componentAwakeMethod);
	void MarkGameObjectForDeletion(GameObject* gameObjectPtr);

protected:
	std::vector<GameObject*> gameObjects;
	std::vector<GameObject*> gameObjectsMarkedForDeletion;

private:
	MulticastDelegate<> onAwake = MulticastDelegate<>(true);

	void DeleteGameObjects();
	void DeleteGameObject(GameObject* gameObjectPtr);


	nlohmann::json Serialize() const;
};

#include "Scene.inl"