#pragma once

#include <vector>

#include "Module.hpp"
#include "Modules/ScenesManager.hpp"

class Engine {
    
public:
    Engine();
    ~Engine();

#ifdef STANDALONE_MODE
    void Run() {

        CreateModules();

        Init();
#else
    void Run() {
#endif

        while (ShouldRun()) {
            Update();
            Render();
        }
#ifdef  STANDALONE_MODE
        Shutdown();
#endif
    }

    Engine* Init();
    void CreateModules();
    void CreateModulesWithEditor();
	void Update();
	void Render();
	void Shutdown();

    static Engine* GetInstance();

    template <typename ModuleType> static ModuleType* GetModule() {

        ModuleType* correctModule = nullptr;
        for (Module* module : instance->modules) {

            if ((correctModule = dynamic_cast<ModuleType*>(module)) != nullptr) {
                return correctModule;
            }
        }

        __assume(false);
    }
#ifdef DEBUG
	static bool debugModeEnabled;
#endif

    bool ShouldRun() const;
private:
    static Engine* instance;
    class Window* window = nullptr;



	std::vector<Module*> modules;
	template<typename ModuleType> ModuleType* CreateModule() {
        ModuleType* module = new ModuleType();
		modules.push_back(module);
        return module;
	}

};