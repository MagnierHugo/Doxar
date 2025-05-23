#include "GameObject.hpp"
#include "Component.hpp"
#include "Components/Transform.hpp"
#include "Components/Renderable.hpp"

GameObject::GameObject(Scene* parentScene_, const std::string& name_, bool enabled_) {
	name = name_;
	enabled = enabled_;
	parentScene = parentScene_;
}

void GameObject::Update() {

	for (GameObject* child : children) {
		child->Update();
	}

	for (std::shared_ptr<Component> component : components) {
		component->Update();
	}

#ifdef DEBUG
	if (trackedAs == "") { return; }
	std::cout << trackedAs << " was updated\n";
#endif
}

void GameObject::Render() {
	
	std::shared_ptr<Renderable> renderablePtr = nullptr;
	for (std::shared_ptr<Component> component : components) {

		renderablePtr = std::dynamic_pointer_cast<Renderable>(component);
		if (renderablePtr == nullptr) { continue; }
		
		 renderablePtr->Render();
	}

#ifdef DEBUG
	if (trackedAs == "") { return; }
	std::cout << trackedAs << " was rendered\n";
#endif
}

#ifdef DEBUG
void GameObject::OnDebug() {

	for (GameObject* child : children) {
		child->OnDebug();
	}

	for (std::shared_ptr<Component> component : components) {

		component->OnDebug();

		if (false) { // if selected
			component->OnDebugSelected();
		}
	}
}
#endif

void GameObject::OnDestroy() {

	for (GameObject* child : children) {
		child->OnDestroy();
	}

	for (std::shared_ptr<Component> component : components) {
		component->OnDestroy();
	}

#ifdef DEBUG
	if (trackedAs == "") { return; }
	std::cout << trackedAs << " was destroyed\n";
#endif
}

#ifdef DEBUG
GameObject* GameObject::AddChild(const std::string& name, const std::string& trackLifeCycleAs, const bool startActive) {

	GameObject* newGameObjectPtr = new GameObject(parentScene, name, startActive);

	newGameObjectPtr->SetTransform(newGameObjectPtr->AddComponent<Transform>().get());
	newGameObjectPtr->parent = this;

	children.push_back(newGameObjectPtr);

	if (trackLifeCycleAs != "") {
		newGameObjectPtr->trackedAs = trackLifeCycleAs;
		std::cout << "Created " << trackLifeCycleAs << "\n";
	}

	return newGameObjectPtr;
}
#else
GameObject* GameObject::AddChild(const std::string& name, const bool startActive) {

	GameObject* newGameObjectPtr = new GameObject(parentScene, name, startActive);

	newGameObjectPtr->SetTransform(newGameObjectPtr->AddComponent<Transform>().get());
	newGameObjectPtr->parent = this;

	children.push_back(newGameObjectPtr);

	return newGameObjectPtr;
}
#endif

void GameObject::SetTransform(Transform* transform_) {
	transform = transform_;
}

nlohmann::json GameObject::Serialize() const {
	nlohmann::json componentsJson = nlohmann::json::array();
	for (std::shared_ptr<Component> component : components) {
		componentsJson.push_back(component->Serialize());
	}

	return {
		{ "GameObjectName", name },
		{ "Enabled", enabled },
		{ "Components", componentsJson }
	};
}

void GameObject::RemoveComponent(int id) {
	for (std::shared_ptr<Component> component : components) {
		if (component->id == id) { continue; }

		components.erase(std::find(components.begin(), components.end(), component));
		return;
	}
}

bool GameObject::TryRemoveComponent(int id) {
	for (std::shared_ptr<Component> component : components) {
		if (component->id != id) { continue; }

		components.erase(std::find(components.begin(), components.end(), component));
		return true;
	}
	return false;
}