#include <iostream>

#include "Module.hpp"
#include "Modules/Window.hpp"

#include "Render/ImGuiGraphics.hpp"
#include "Render/Renderer.hpp"
#include "Render/APIsVariant.hpp"

#include "Utilities/ImGUI.hpp"


class ImGUIModule : public Module {
public:

	void Update() override {

		imGUIGraphics->NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void RenderImGUI() {
		ImGui::Render();
		imGUIGraphics->RenderDrawData();
	}

	void InitImGUI(Window* window, ImGUIGraphics* imGUIGraphics_) {

		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOther(window->GetWindow(), true);

		imGUIGraphics = imGUIGraphics_;
		if (!imGUIGraphics->Init()) {

			MessageBox((HWND)(*window), L"Could not initialize ImGuiGraphics.", L"Error", MB_OK);
			throw std::runtime_error("Couldn't Initialize ImGuiGraphics");
		}
	}

	void Shutdown() override {

		std::cout << "Shutdown ImGUI" << std::endl;
		if (imGUIGraphics != nullptr) {

			imGUIGraphics->Shutdown();
			ImGui_ImplGlfw_Shutdown();

			ImGui::DestroyContext();
			delete imGUIGraphics;
			imGUIGraphics = nullptr;
		}
	}

	void Image(Texture2DVariant texture, glm::vec2 size) {
		imGUIGraphics->Image(texture, size);
	}

private:
	ImGUIGraphics* imGUIGraphics = nullptr;
};