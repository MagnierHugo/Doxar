#pragma once

#include <iostream>

#include "Core/Maths/vec2.hpp"
#include "Core/Maths/vec3.hpp"
#include "Core/Maths/gtc/quaternion.hpp"

#include "Engine.hpp"
#include "Modules/Window.hpp"
#include "Scene.hpp"
#include "Component.hpp"
#include "GameObject.hpp"
#include "Modules/Time.hpp"
#include "Components/MeshRenderer.hpp"
#include "Components/Transform.hpp"
#include "Components/Camera.hpp"
#include "Modules/InputManager.hpp"
#include "Utilities/ImGUI.hpp"
#include "Modules/RenderModule.hpp"

class DefaultScene : public Scene {

public:
	DefaultScene(bool startActive) : Scene("DefaultScene", startActive) {}

	void Init() override {

		RenderModule::CreateTexture("Assets/default_texture.png");

		GameObject* camera = AddGameObjectUntracked("CameraGO", true);
		camera->AddComponent<Camera>()->MakeMainCamera();
		camera->transform->position = glm::vec3(.0f, .0f, -5.f);

		GameObject* katana = AddGameObjectUntracked("Katana", true);
		katana->AddComponent<MeshRenderer>()->LoadModel("Assets/3D_Models/katana_V2.obj");
		katana->transform->position += glm::vec3(.0f, 1.5f, 3.0f); // I have the highground
		katana->transform->scale += glm::vec3(.0f, 5.0f, 5.0f); // big boi  
		// katana s default orientation is pointed along the X axis by default so yes the axises don t really make sense
		katana->transform->rotation = glm::quat(0.7071f, 0.0f, 0.0f, 0.7071f); // erect

		GameObject* creature = AddGameObjectUntracked("Creature", true);
		creature->AddComponent<MeshRenderer>()->LoadModel("Assets/3D_Models/Alien_Animal/Test_Alien-Animal-Blender_2.81.fbx");
		creature->transform->position = glm::vec3(.0f, .0f, .0f);
		creature->transform->rotation = glm::quat(0.7071f, -0.7071f, 0.0f, 0.0f);
		creature->transform->scale = glm::vec3(.1f, .1f, .1f); // smol

		GameObject* lakeScene = AddGameObjectUntracked("Lake scene", true);
		lakeScene->AddComponent<MeshRenderer>()->LoadModel("Assets/3D_Models/Low_Poly_Lake/untitled.fbx");
		lakeScene->transform->position = glm::vec3(-2.f, .0f, .0f);
		lakeScene->transform->rotation = glm::quat(0.7071f, -0.7071f, .0f, .0f);
		//lakeScene->transform->scale = glm::vec3(.25f, .25f, .25f);

	}

private:

};