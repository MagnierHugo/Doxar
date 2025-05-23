#pragma once

#include "Scene.hpp"

//template <typename ...ScenesNames> static void ScenesManager::LoadScenes(bool additive) {
//
//	if (!additive) {
//		UnloadAllScenes();
//	}
//
//	(LoadScene<ScenesNames>(true), ...);
//}
//
//template <typename SceneName> static void ScenesManager::UnloadScene() {
//
//	instance->scenes[typeid(SceneName).name()]->active = false;
//}
//
//template <typename ...ScenesNames> static void ScenesManager::UnloadScenes() {
//
//	(UnloadScene<ScenesNames>(), ...);
//}
