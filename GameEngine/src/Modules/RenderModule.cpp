#include <iostream>

#include "Engine.hpp"

#include "Modules/RenderModule.hpp"
#include "Modules/Window.hpp"

#include "Render/Dx11/Dx11TextureShader.hpp"
#include "Render/Dx11/Dx11Renderer.hpp"
#include "Render/Dx11/Dx11ImGuiGraphics.hpp"
#include "Render/Dx11/Dx11Mesh.hpp"

#include "Resources/ResourceManager.hpp"
#include "Resources/Dx11/Dx11Texture.hpp"

#include "Components/Renderable.hpp"
#include "Components/Camera.hpp"

#include "Modules/ImGUIModule.hpp"
#include "Utilities/ImGUI.hpp"
#include "Utilities/Logger.hpp"

#include "Common.hpp"

RenderModule* RenderModule::instance = nullptr;


RenderModule::RenderModule() {
	instance = this;
}

RenderModule* RenderModule::GetInstance() {
	return instance;
}

RenderModule::RenderingAPI RenderModule::SelectAPI() {
	static RenderModule::RenderingAPI selectedAPI = RenderModule::RenderingAPI::Unknown;

	if (selectedAPI != RenderingAPI::Unknown) { return selectedAPI; }

	/* Actual selection code instead */
	selectedAPI = RenderingAPI::Dx11; // I don t get why not just { static RenderingAPI selectedAPI = RenderingAPI::TheOneIWant; } 

	return selectedAPI;
}

Renderer* RenderModule::CreateRenderer() {
	switch (SelectAPI()) {
	case RenderModule::RenderingAPI::Dx12:
	case RenderModule::RenderingAPI::Vulkan:
	case RenderModule::RenderingAPI::OpenGL:
	case RenderModule::RenderingAPI::Dx11:
		return new Dx11Renderer();
	default:
		LOG_CRITICAL("Unknown Graphics API");
		throw std::runtime_error("Unknown Graphics API");
	}
}

Shader* RenderModule::CreateShader() {
	switch (SelectAPI()) {
	case RenderModule::RenderingAPI::Dx12:
	case RenderModule::RenderingAPI::Vulkan:
	case RenderModule::RenderingAPI::OpenGL:
	case RenderModule::RenderingAPI::Dx11:
		return new Dx11TextureShader();
	default:
		LOG_CRITICAL("Unknown Graphics API");
		throw std::runtime_error("Unknown Graphics API");
	}
}

std::shared_ptr<Texture> RenderModule::CreateTexture(const std::string& filePath) {
	switch (RenderModule::SelectAPI()) {
	case RenderModule::RenderingAPI::Vulkan:
	case RenderModule::RenderingAPI::OpenGL:
	case RenderModule::RenderingAPI::Dx12:
	case RenderModule::RenderingAPI::Dx11:
		return std::static_pointer_cast<Texture>(ResourceManager::Load<Dx11Texture>(filePath));
	default:
		throw std::runtime_error("Unknow Graphics API");
	}
}

Mesh* RenderModule::CreateMesh(std::vector<Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<std::shared_ptr<Texture>> textures_) {
	switch (RenderModule::SelectAPI()) {
	case RenderModule::RenderingAPI::Vulkan:
	case RenderModule::RenderingAPI::OpenGL:
	case RenderModule::RenderingAPI::Dx12:
	case RenderModule::RenderingAPI::Dx11:
		return new Dx11Mesh(vertices_, indices_, textures_);
	default:
		LOG_CRITICAL("Unknown Graphics API");
		throw std::runtime_error("Unknown Graphics API");
	}
}

ImGUIGraphics* RenderModule::CreateImGuiGraphics() {
	switch (SelectAPI()) {
	case RenderModule::RenderingAPI::Dx12:
	case RenderModule::RenderingAPI::Vulkan:
	case RenderModule::RenderingAPI::OpenGL:
	case RenderModule::RenderingAPI::Dx11:
		return new Dx11ImGuiGraphics();
	default:
		LOG_CRITICAL("Unknown Graphics API");
		throw std::runtime_error("Unknown Graphics API");
	}
}


void RenderModule::Init() {

	Window* window = Engine::GetModule<Window>();

	renderer = CreateRenderer();
	if (!renderer->Init(window, window->GetSize().x, window->GetSize().y)) {

		MessageBox(*window, L"Could not initialize Direct3D", L"Error", MB_OK);
		LOG_CRITICAL("Couldn't Initialize Renderer");
		throw std::runtime_error("Couldn't Initialize Renderer");
	}

	Renderable::SetRenderer(renderer);

	imGUIModule = Engine::GetModule<ImGUIModule>();

	imGUIModule->InitImGUI(window, CreateImGuiGraphics());
}

void RenderModule::Update() { // prepare window in order to draw on it later in RenderModule::Render()

	renderer->Clear(0.2f, 0.3f, 0.8f);
}

void RenderModule::Render() {
	//mainCamera != nullptr ? mainCamera->Render() : []() { MessageBox(*Engine::GetModule<Window>(), L"No camera", L"Error", MB_OK); throw std::runtime_error("no camera"); }();
	renderer->StartRenderToMainView();
	imGUIModule->RenderImGUI();
	renderer->EndRenderToMainView();

	renderer->Present();
}

void RenderModule::Shutdown() {

	mainCamera = nullptr;

	if (renderer != nullptr) {
		renderer->Shutdown();
		delete renderer;
		renderer = nullptr;
	}

	/*if (imGUIGraphics != nullptr) {
		delete imGUIGraphics;
		imGUIGraphics = nullptr;
	}*/
}

//void RenderModule::InitImGUI(Window* window) {
//
//	ImGui::CreateContext();
//
//	ImGui::StyleColorsDark();
//
//	ImGui_ImplGlfw_InitForOther(window->GetWindow(), true);
//
//	imGUIGraphics = CreateImGuiGraphics();
//	if (!imGUIGraphics->Init()) {
//
//		MessageBox(*window, L"Could not initialize ImGuiGraphics.", L"Error", MB_OK);
//		LOG_CRITICAL("Couldn't Initialize ImGuiGraphics!");
//		throw std::runtime_error("Couldn't Initialize ImGuiGraphics");
//	}
//}

//void RenderModule::CleanupImGUI() {
//
//	imGUIGraphics->Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//
//	ImGui::DestroyContext();
//}

void RenderModule::SubscribeCamera(Camera* camera) {
	instance->availableCameras.push_back(camera);
}

void RenderModule::UnSubscribeCamera(Camera* camera) {

	instance->availableCameras.erase(
		std::find(
			instance->availableCameras.begin(),
			instance->availableCameras.end(),
			camera
		)
	);

	// if the camera that just got removed was the main one
	if (camera == instance->mainCamera) {

		instance->mainCamera = instance->availableCameras.empty() ? nullptr : instance->availableCameras[0]; // try ot fall back on the next one if there is one
	}

}

void RenderModule::MakeMainCamera(Camera* camera) {
	
	//// if mainCamera is removed (fed nullptr to RenderModule::MakeMainCamera() )
	//instance->mainCamera = camera == nullptr ?
	//		instance->availableCameras.empty() ? nullptr : instance->availableCameras[0]: // fallback if any mainCamera available else no mainCamera
	//		camera 
	//;

	instance->mainCamera = camera;

}
