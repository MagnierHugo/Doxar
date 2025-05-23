#pragma once

#include <string>
#include <unordered_map>
#include <windows.h>
#include <libloaderapi.h>

#include "Module.hpp"


class ScriptManager : public Module {

	friend class Engine;

public:
	void Init() override;
	void Update() override;
	void Shutdown() override;

	void MonitorScriptChanges();
	void StopMonitoringDirectory();
	static void LoadScript(class GameObject* gameObject, const std::string& file);

private:
	static ScriptManager* instance;
	std::unordered_map<std::string, HMODULE> libraryHandles;

	ScriptManager() {
		instance = this;
	}

	//void LoadScriptLibrary(const std::string& libraryPath);
	//void UnloadScriptLibrary(const std::string& libraryPath);
	void LoadLibraries();
	void UnloadLibraries();

};