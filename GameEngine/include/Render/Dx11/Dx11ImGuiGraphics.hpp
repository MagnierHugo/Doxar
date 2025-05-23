#pragma once

#include "Render/ImGuiGraphics.hpp"


class Dx11ImGuiGraphics : public ImGUIGraphics {
public:
	Dx11ImGuiGraphics() = default;
	Dx11ImGuiGraphics(const Dx11ImGuiGraphics&) = default;
	~Dx11ImGuiGraphics() = default;

	bool Init() const override;
	void Shutdown() const override;

	void NewFrame() const override;
	void RenderDrawData() const override;

	void Image(Texture2DVariant texture, glm::vec2 size) const override;
};