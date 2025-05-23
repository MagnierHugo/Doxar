#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Module.hpp"


class RenderModule : public Module {

	friend class Engine;

public:

	enum class RenderingAPI {
		Dx11,
		Dx12,
		Vulkan,
		OpenGL,

		Unknown
	};

	RenderModule(const RenderModule&) = delete;
	RenderModule(RenderModule&&) = delete;
	RenderModule& operator=(const RenderModule&) = delete;
	RenderModule& operator=(RenderModule&&) = delete;

	static RenderModule* GetInstance();

	static RenderingAPI SelectAPI();
	static class Renderer* CreateRenderer();
	static class ImGUIGraphics* CreateImGuiGraphics();
	static class Shader* CreateShader();
	static std::shared_ptr<class Texture> CreateTexture(const std::string& filePath);
	static class Mesh* CreateMesh(std::vector<struct Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<std::shared_ptr<Texture>> textures_);


	void Init() override;
	void Update() override;
	void Render() override;
	void Shutdown() override;

	static void SubscribeCamera(class Camera* camera);
	static void UnSubscribeCamera(Camera* camera);
	static void MakeMainCamera(Camera* camera);

	class Renderer* renderer = nullptr;
	Camera* mainCamera = nullptr;
private:
	static RenderModule* instance;
	std::vector<Camera*> availableCameras;
	class ImGUIModule* imGUIModule = nullptr;

	RenderModule();
	~RenderModule() = default;
};