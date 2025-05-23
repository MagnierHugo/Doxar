
#include "Engine.hpp"

#include "Modules/ImGUIModule.hpp"
#include "Modules/RenderModule.hpp"
#include "Modules/AudioManager.hpp"
#include "Modules/Time.hpp"
#include "Modules/EditorWindows.hpp"
#include "Modules/ScriptManager.hpp"

#include "Core/Maths/glm.hpp"
#include "Core/Maths/gtc/quaternion.hpp"

#include "Components/Transform.hpp"
#include "Components/MeshRenderer.hpp"

#include "Utilities/Logger.hpp"

class Editor {

public:

	void Run() {

		Init();

		while (engine->ShouldRun()) {
			Update();
			Render();
		}

		Shutdown();
	}

	void Init() {
		Logger::Init();
	
		engine = new Engine();
		engine->CreateModulesWithEditor();
		engine->Init();

		CreateUpdateList<Window, InputManager, ScriptManager, Time, ImGUIModule, EditorWindows, RenderModule, AudioManager>();
		
		engine->GetModule<ScenesManager>()->ParseScenes();

	}

	void Update() {

		if (InputManager::GetKeyDown(GLFW_KEY_I)) {
			engine->GetModule<Time>()->Init();
			engine->Run();
		}
		
		if (InputManager::GetKeyDown(GLFW_KEY_M)) {
			engine->GetModule<ScenesManager>()->SaveScenes();
		}
		
		if (InputManager::GetKeyDown(GLFW_KEY_P)) {
			Scene* scene = engine->GetModule<ScenesManager>()->GetScene("SceneName1");
			if (scene == nullptr) {
				LOG_DEBUG("Scene is nullptr");
			}
			else {
				LOG_DEBUG("Creating the creature");
				GameObject* creature = scene->AddGameObjectUntracked("Creature", true);
				creature->AddComponent<MeshRenderer>()->LoadModel("Assets/3D_Models/Alien_Animal/Test_Alien-Animal-Blender_2.81.fbx");
				creature->transform->position = glm::vec3(.0f, .0f, .0f);
				creature->transform->rotation = glm::quat(0.7071f, -0.7071f, 0.0f, 0.0f);
				creature->transform->scale = glm::vec3(.1f, .1f, .1f); // smol
			}
		}

		for (Module* module : updateList) {
			module->Update();
		}
	}

	void Render() {
		engine->Render();
	}

	void Shutdown() {
		LOG_DEBUG("Editor Shutdown");
		engine->Shutdown();

		delete engine;
		engine = nullptr;
	}

	template <typename ...ModuleType> void CreateUpdateList() {

		(updateList.push_back(engine->GetModule<ModuleType>()), ...);
	}

	Engine* engine = nullptr;
private:
	std::vector<Module*> updateList;
};