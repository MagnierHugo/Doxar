#pragma once

#include "Engine.hpp"

#include "Component.hpp"

#include <filesystem>
#include <memory>
#include <unordered_map>
#include <unordered_set>

#include "Modules/AudioManager.hpp"

#include "Modules/ScenesManager.hpp"
#include "Modules/ScriptManager.hpp"
#include "Utilities/ImGUI.hpp"

#include "Components/Transform.hpp"
#include "Components/MeshRenderer.hpp"
#include "Components/Camera.hpp"
#include "Components/ScriptAdder.hpp"

#define StripClassPrefix(name) (strncmp(name, "class ", 6) == 0 ? (name + 6) : name)


struct FileNode {
	std::string name;
	std::string path;
	bool is_directory;
	std::vector<std::shared_ptr<FileNode>> children;
	std::shared_ptr<FileNode> sourceDirectory;

};

class EditorWindows : public Module {
public:

	void Init() override;
	void Update() override;
	void UpdateCurrentFolder();

	//void Shutdown() override;

private:
	void GenerateFileTree(std::shared_ptr<FileNode> node);
	void DisplayFileTree(std::shared_ptr<FileNode> node);
	void NavMenuArbo();

	void DrawFileName(std::string text);
	void SongIcon(std::shared_ptr<FileNode> child);
	void ObjectIcon(std::shared_ptr<FileNode> child);
	void DirectoryIcon(std::shared_ptr<FileNode> child);
	void PictureFile(std::shared_ptr<FileNode> child);
	int SetCurrentPath(std::string name, std::string path, std::shared_ptr<FileNode> node);

	void HierarchyWindow();

	void GraphicWindows();

	//Ref
	AudioManager* audiomanager = nullptr;
	ScenesManager* scenesManager = nullptr;
	GameObject* inspectedGameObject = nullptr;
	//

	std::shared_ptr<FileNode> currentNode;
	std::shared_ptr<FileNode> rootNode;

	//Display
	int maxCharacter = 20;
	ImVec2 sizeDirectoryIcon = ImVec2(70, 70);
	ImVec2 sizeSongIcon = ImVec2(70, 70);
	ImVec2 sizePictureIcon = ImVec2(70, 70);
	ImVec2 sizeObjectIcon = ImVec2(70, 70);
	//

	std::unordered_set<std::string> expandedScenes;
	int savedComponentIndex = 0;
};