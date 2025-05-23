#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "Engine.hpp"

#include "Components/Renderable.hpp"

#include "Render/Shader.hpp"


template<typename T>
concept _IsShader = std::is_base_of<Shader, T>::value;


class MeshRenderer : public Renderable {

public:
	MeshRenderer();
	void Render() const override;
	void OnDestroy();

	std::string fileName;
	void LoadModel(const std::string& fileName_);
	std::vector<class Mesh*> GetMeshes() const;

	template<typename T> requires(_IsShader<T>) void SetShader() {
		if (shader != nullptr) { delete shader; }
		shader = new T();
		shader->Initialize(Engine::GetModule<Window>());
	}

private:
	std::shared_ptr<class Model> model;
	Shader* shader;

	void OnInspect() override {
		ImGUI::BeginChild("MeshRendererComponent");

		char buffer[256]; // ImGui needs a fixed-size buffer
		strncpy_s(buffer, fileName.c_str(), sizeof(buffer));
		buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

		if (ImGui::InputText("FilePath", buffer, sizeof(buffer))) {
			fileName = buffer; // Update the string if it changed
		}
		if (ImGUI::Button("LoadMesh")) {
			LoadModel(fileName);
		}
		ImGUI::EndChild();
	}

	nlohmann::json Serialize() override {
		return {
			{ "ComponentType",typeid(MeshRenderer).name() },
			{ "FileName", fileName }
		};
	}

	void Deserialize(nlohmann::json json) override {
		LoadModel(json["FileName"]);
	}

private:
	static ComponentRegistrer<MeshRenderer> componentRegistrer;
};