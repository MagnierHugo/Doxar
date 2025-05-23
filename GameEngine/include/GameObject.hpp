#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include <string>

#include "Utilities/JSON.hpp"

#ifdef DEBUG
	#define AddChildTracked(name, startActive, trackAs) AddChild(name, trackAs, startActive)
	#define AddChildUntracked(name, startActive) AddChild(name, "", startActive)
#else
	#define AddChildTracked(name, startActive, trackAs) AddChild(name, startActive)
	#define AddChildUntracked(name, startActive) AddChild(name, startActive)
#endif

class GameObject {
	friend class EditorWindows;
	friend class ScenesManager;
public:
	GameObject(class Scene* parentScene_, const std::string& name_ = "GameObject", bool enabled_ = true);

	bool enabled = true;
	std::string name = "";
	Scene* parentScene = nullptr;
#ifdef DEBUG
	std::string trackedAs = "";
#endif
	class Transform* transform = nullptr;
	void SetTransform(Transform* transform_);


	template<typename TComponent, typename... Args> std::shared_ptr<TComponent> AddComponent(Args&&... args);
	template<typename TComponent> void AddComponent(TComponent* component);
	template<typename TComponent> bool TryGetComponent(std::shared_ptr<TComponent>& returnPtr, int skipOccurences = 0);
	template<typename TComponent> bool TryGetComponents(std::vector<std::shared_ptr<TComponent>>& returnPtrArray);
	template<typename TComponent> std::shared_ptr<TComponent> GetComponent(int skipOccurences = 0);
	template<typename TComponent> std::vector<std::shared_ptr<TComponent>> GetComponents();
	template<typename TComponent> void RemoveComponent(int skipOccurences = 0);
	template<typename TComponent> bool TryRemoveComponent(int skipOccurences = 0);

	void RemoveComponent(int id);
	bool TryRemoveComponent(int id);

	void Disable();
	void Enable();

	void Update();
	void Render();
#ifdef DEBUG
	void OnDebug();
#endif
	void OnDestroy();

#ifdef DEBUG
	GameObject* AddChild(const std::string& name, const std::string& trackLifeCycleAs, const bool startActive = true);
#else
	GameObject* AddChild(const std::string& name, const bool startActive = true);
#endif

	std::vector<GameObject*> children;
	GameObject* parent = nullptr;

	nlohmann::json Serialize() const;
private:
	std::vector<std::shared_ptr<class Component>> components;
};

#include "GameObject.inl"