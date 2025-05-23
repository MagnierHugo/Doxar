#include "Render/Dx11/Dx11ImGuiGraphics.hpp"

#include <d3d11.h>
#include <stdexcept>

#include "Modules/RenderModule.hpp"
#include "Render/Renderer.hpp"

#include "Utilities/ImGUI.hpp"
#include "Utilities/Logger.hpp"


bool Dx11ImGuiGraphics::Init() const {
	DeviceVariant deviceVariant = RenderModule::GetInstance()->renderer->GetDevice();
	ID3D11Device** ptrDevice = std::get_if<ID3D11Device*>(&deviceVariant);

	DeviceContextVariant deviceContextVariant = RenderModule::GetInstance()->renderer->GetDeviceContext();
	ID3D11DeviceContext** ptrDeviceContext = std::get_if<ID3D11DeviceContext*>(&deviceContextVariant);

	if (ptrDevice != nullptr && ptrDeviceContext != nullptr) {
		return ImGui_ImplDX11_Init(*ptrDevice, *ptrDeviceContext);
	}

	LOG_CRITICAL("Wrong API Device or Device Context Variant");
	throw std::runtime_error("Wrong API Device or Device Context Variant");
}

void Dx11ImGuiGraphics::Shutdown() const {
	ImGui_ImplDX11_Shutdown();
}

void Dx11ImGuiGraphics::NewFrame() const {
	ImGui_ImplDX11_NewFrame();
}

void Dx11ImGuiGraphics::RenderDrawData() const {
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Dx11ImGuiGraphics::Image(Texture2DVariant texture, glm::vec2 size) const {
	DeviceVariant deviceVariant = RenderModule::GetInstance()->renderer->GetDevice();
	ID3D11Device** ptrDevice = std::get_if<ID3D11Device*>(&deviceVariant);
	if (ptrDevice == nullptr) {
		LOG_ERROR("Wrong API Device Variant");
		return;
	}

	ID3D11Texture2D** ptrTexture = std::get_if<ID3D11Texture2D*>(&texture);
	if (ptrTexture == nullptr) {
		LOG_ERROR("Wrong API Texture2D Variant");
		return;
	}

	ID3D11ShaderResourceView* srv = nullptr;
	(*ptrDevice)->CreateShaderResourceView(*ptrTexture, nullptr, &srv);

	ImGUI::Image(reinterpret_cast<ImTextureID>(srv), ImVec2(size.x, size.y));
}