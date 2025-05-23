#pragma once

#include <iostream>
#include <unordered_map>

#include "Utilities/MulticastDelegate.hpp"
#include "Module.hpp"
#include "Scene.hpp"


class ScenesManager : public Module {
	friend class EditorWindows;
public:
	ScenesManager();

	void Init() override;
	void Update() override;
	void Render() override;
	void Shutdown() override;

	static void AddScene(const std::string& name_, bool doStartActive = false);
	static void RemoveScene(const std::string& name_);
	static void LoadScene(const std::string& name_, bool additive = false);
	/*template <typename ...ScenesNames> static void LoadScenes(bool additive = false);
	template <typename SceneName> static void UnloadScene();
	template <typename ...ScenesNames> static void UnloadScenes();*/

	void UnloadAllScenes();

	static Scene* GetScene(const std::string& name_);

	static Scene* persistentScene;
	void ForceInitScenes();
	bool ParseScenes();
	bool SaveScenes();
private:
	static ScenesManager* instance;
	std::unordered_map<std::string, Scene*> scenes;
	MulticastDelegate<> initScenes = MulticastDelegate(true);
	class RenderModule* renderModule = nullptr;
};

#include "Modules/ScenesManager.inl"