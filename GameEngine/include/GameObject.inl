#pragma once

#include <iostream>
#include <algorithm>

#include "assert.h"

#include "Scene.hpp"
#include "Component.hpp"


inline void GameObject::Disable() {

	for (GameObject* child : children) {
		child->Disable();
	}

	for (std::shared_ptr<Component> component : components) {
		component->Disable();
	}

	enabled = false;

#ifdef DEBUG
	if (trackedAs == "") { return; }
	std::cout << trackedAs << " was disabled\n";
#endif
}

inline void GameObject::Enable() {

	for (GameObject* child : children) {
		child->Enable();
	}

	for (std::shared_ptr<Component> component : components) {
		component->Enable();
	}

	enabled = true;

#ifdef DEBUG
	if (trackedAs == "") { return; }
	std::cout << trackedAs << " was enabled\n";
#endif
}

// Only throws when TComponent does not inherit component (or the GameObject s parent scene is nullptr)
template<typename TComponent, typename... Args> std::shared_ptr<TComponent> GameObject::AddComponent(Args&&... args) {

	std::shared_ptr<TComponent> componentPtr = std::make_shared<TComponent>(std::forward<Args>(args)...);
	std::shared_ptr<Component> asComponent = std::static_pointer_cast<Component>(componentPtr);

	asComponent->SetOwner(this);

	components.push_back(asComponent);

	parentScene->QueueAwake([=]() { asComponent->Awake(); });

	return componentPtr;
}

template<typename TComponent> void GameObject::AddComponent(TComponent* component) {
	std::shared_ptr<TComponent> componentPtr = std::shared_ptr<TComponent>(component);
	std::shared_ptr<Component> asComponent = std::static_pointer_cast<Component>(componentPtr);
	components.push_back(asComponent);
	parentScene->QueueAwake([=]() { asComponent->Awake(); });
}

template<typename TComponent> bool GameObject::TryGetComponent(std::shared_ptr<TComponent>& returnPtr, int skipOccurences) {

	returnPtr = nullptr;
	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		if (skipOccurences-- > 0) { continue; }

		returnPtr = relevantComponent;
		return true;
	}

	return false;
}

template<typename TComponent> bool GameObject::TryGetComponents(std::vector<std::shared_ptr<TComponent>>& returnPtrArray) {

	bool gotAtLeastOne = false;
	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		returnPtrArray.push_back(relevantComponent);

		gotAtLeastOne = true;
	}

	return gotAtLeastOne;
}

template<typename TComponent> std::shared_ptr<TComponent> GameObject::GetComponent(int skipOccurences) {

	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		if (skipOccurences-- > 0) { continue; }

		return relevantComponent;
	}

	return nullptr;
}

template<typename TComponent> std::vector<std::shared_ptr<TComponent>> GameObject::GetComponents() {
	
	std::vector<std::shared_ptr<TComponent>> toReturn;

	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		toReturn.push_back(relevantComponent);
	}
	
	return toReturn;
}

template<typename TComponent> void GameObject::RemoveComponent(int skipOccurences) {

	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		if (skipOccurences-- > 0) { continue; }

		components.erase(std::find(components.begin(), components.end(), component));

		return;
	}
}

template<typename TComponent> bool GameObject::TryRemoveComponent(int skipOccurences) {

	std::shared_ptr<TComponent> relevantComponent = nullptr;
	for (std::shared_ptr<Component> component : components) {

		relevantComponent = std::dynamic_pointer_cast<TComponent>(component);
		if (relevantComponent == nullptr) { continue; }

		if (skipOccurences-- > 0) { continue; }

		components.erase(std::find(components.begin(), components.end(), component));

		return true;
	}

	return false;
}

