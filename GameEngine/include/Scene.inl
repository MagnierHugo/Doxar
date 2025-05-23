#pragma once


#include "GameObject.hpp"
#include "Scene.hpp"

inline void Scene::MarkGameObjectForDeletion(GameObject* gameObjectPtr) {
	gameObjectsMarkedForDeletion.push_back(gameObjectPtr);
}

inline void Scene::DeleteGameObjects() {

	for (GameObject* toDeletePtr : gameObjectsMarkedForDeletion) {
		DeleteGameObject(toDeletePtr);
	}

	gameObjectsMarkedForDeletion.clear();
}

inline void Scene::DeleteGameObject(GameObject* gameObjectPtr) {

	if (gameObjectPtr->parent != nullptr) {

		gameObjectPtr->parent->children.erase(
			std::find(
				gameObjectPtr->parent->children.begin(), gameObjectPtr->parent->children.end(),
				gameObjectPtr
			)
		);
	}

	for (int i = static_cast<int>(gameObjectPtr->children.size() - 1); i > -1; i--) {
		gameObjectPtr->children[i]->OnDestroy();
		delete gameObjectPtr->children[i];
	}

	gameObjectPtr->children.clear();



	gameObjects.erase(std::find(gameObjects.begin(), gameObjects.end(), gameObjectPtr));

	gameObjectPtr->OnDestroy();

	delete gameObjectPtr;
}