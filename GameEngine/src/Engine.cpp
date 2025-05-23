#include "Engine.hpp"

#include "Modules/InputManager.hpp"
#include "Modules/ScenesManager.hpp"
#include "Modules/Time.hpp"
#include "Modules/AudioManager.hpp"
#include "Modules/Window.hpp"
#include "Modules/RenderModule.hpp"
#include "Modules/ImGUIModule.hpp"
#include "Modules/EditorWindows.hpp"
#include "Modules/ScriptManager.hpp"

#include "Utilities/ImGUI.hpp"
#include "Utilities/Logger.hpp"

Engine* Engine::instance = nullptr;
#ifdef DEBUG
bool Engine::debugModeEnabled = false;
#endif


Engine::Engine() {
    if (instance != nullptr) { throw std::runtime_error("Engine instance already exists!"); }

    instance = this;
}

Engine::~Engine() { }

Engine* Engine::GetInstance() {
    return instance;
}

bool Engine::ShouldRun() const {
    return !window->ShouldClose();
}

void Engine::CreateModules() {
    window = CreateModule<Window>();
	_ = CreateModule<InputManager>();
    _ = CreateModule<Time>();
    _ = CreateModule<ImGUIModule>();
    _ = CreateModule<ScenesManager>();
	_ = CreateModule<RenderModule>();
	_ = CreateModule<AudioManager>();


	std::cout << "CreateModules done\n";
}

void Engine::CreateModulesWithEditor() {
	window = CreateModule<Window>();
	_ = CreateModule<InputManager>();
	_ = CreateModule<ScriptManager>();
	_ = CreateModule<Time>();
	_ = CreateModule<ImGUIModule>();
	_ = CreateModule<ScenesManager>();
	_ = CreateModule<RenderModule>();
	_ = CreateModule<EditorWindows>();
	_ = CreateModule<AudioManager>();

	LOG_DEBUG("CreateModules done");

}

Engine* Engine::Init() {
	LOG_DEBUG("Init Starting");

	for (Module* module : modules) {
		module->Init();
	}

	LOG_DEBUG("Init Done");
	return this;
}

void Engine::Update() {

	for (Module* module : modules) {
		module->Update();
	}
}

void Engine::Render() {
	
	for (Module* module : modules) {
		module->Render();
	}
}

void Engine::Shutdown() {
	
	for (Module* module : modules) {
		module->Shutdown();
		delete module;
	}
}


