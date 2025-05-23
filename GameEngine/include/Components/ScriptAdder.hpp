#pragma once

#include <string>

#include "Component.hpp"

#include "Modules/ScriptManager.hpp"


class ScriptAdder : public Component {
public:
	ScriptAdder() = default;
	~ScriptAdder() = default;

	nlohmann::json Serialize() override {
		return nlohmann::json();
	}
	void Deserialize(nlohmann::json json) override {}

	void OnInspect() override {
		ImGUI::BeginChild("ScriptComponent");

		char buffer[256]; // ImGui needs a fixed-size buffer
		strncpy_s(buffer, fileName.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

		if (ImGui::InputText("Script Name", buffer, sizeof(buffer))) {
			fileName = buffer; // Update the string if it changed
		}
		if (ImGUI::Button("LoadScript")) {
			ScriptManager::LoadScript(Owner, fileName);
		}
		ImGUI::EndChild();
	}

	std::string fileName;
};