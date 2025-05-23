#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "Utilities/JSON.hpp"
#include "Utilities/ImGUI.hpp"


class Component {
	
public:

	Component() {
		id = rand();
	}

	static std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>>& GetComponentBuilder() {
		static std::unordered_map<std::string, std::function<std::shared_ptr<Component>()>> componentBuilder;
		// this instead of the usual way in order to ensure it being ready when called by other static initializers
		return componentBuilder;
	}
	template <typename ComponentType> class ComponentRegistrer {

	public:
		ComponentRegistrer() {
			std::string componentTypeName = typeid(ComponentType).name();
			std::cout << "Registered ComponentType: " << componentTypeName << std::endl;
			GetComponentBuilder()[componentTypeName] = []() { return std::make_shared<ComponentType>(); };
		}
	};



	class GameObject* Owner = nullptr;
	bool enabled = false;
	int id;

	virtual void Enable();
	virtual void Disable();
	void SetOwner(GameObject* entity);
	class Transform* Transform() const;

	virtual inline void Awake() { }
	virtual inline void Update() { }
#ifdef DEBUG
	virtual inline void OnDebug() { }
	virtual inline void OnDebugSelected() { }
#endif
	virtual inline void OnDestroy() { }

	template<typename TComponent> std::shared_ptr<TComponent> AddComponent();
	template<typename TComponent> bool TryGetComponent(std::shared_ptr<TComponent>& returnPtr, const int skipOccurences = 0);
	template<typename TComponent> bool TryGetComponents(std::vector<std::shared_ptr<TComponent>>& returnPtrArray);
	template<typename TComponent> std::shared_ptr<TComponent> GetComponent(const int skipOccurences = 0);
	template<typename TComponent> std::vector<std::shared_ptr<TComponent>> GetComponents();
	template<typename TComponent> void RemoveComponent(const int skipOccurences = 0);
	template<typename TComponent> bool TryRemoveComponent(const int skipOccurences = 0);


	virtual nlohmann::json Serialize() = 0;
	virtual void Deserialize(nlohmann::json json) = 0;

	virtual void OnInspect() = 0;
};

#include "Component.inl"
