#pragma once

#include "GameObject.hpp"
#include "Components/Transform.hpp"

inline void Component::Enable() {
	enabled = true;
}

inline void Component::Disable() {
	enabled = false;
}

inline void Component::SetOwner(class GameObject* gameObject) {
	Owner = gameObject;
}

inline Transform* Component::Transform() const {
	return Owner->transform;
}


template<typename TComponent> inline std::shared_ptr<TComponent> Component::AddComponent() {
	return Owner->AddComponent<TComponent>();
}

template<typename TComponent> inline bool Component::TryGetComponent(std::shared_ptr<TComponent>& returnPtr, const int skipOccurences) {
	return Owner->TryGetComponent<TComponent>(returnPtr, skipOccurences);
}

template<typename TComponent> inline bool Component::TryGetComponents(std::vector<std::shared_ptr<TComponent>>& returnPtrArray) {
	return Owner->TryGetComponents<TComponent>(returnPtrArray);
}

template<typename TComponent> inline std::shared_ptr<TComponent> Component::GetComponent(const int skipOccurences) {
	return Owner->GetComponent<TComponent>(skipOccurences);
}

template<typename TComponent> inline std::vector<std::shared_ptr<TComponent>> Component::GetComponents() {
	return Owner->GetComponents<Component>();
}

template<typename TComponent> inline void Component::RemoveComponent(const int skipOccurences) {
	Owner->RemoveComponent<TComponent>(skipOccurences);
}

template<typename TComponent> inline bool Component::TryRemoveComponent(const int skipOccurences) {
	return Owner->TryRemoveComponent(skipOccurences);
}
