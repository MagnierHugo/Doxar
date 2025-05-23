#pragma once

#include "Core/Maths/vec2.hpp"

#include "Render/APIsVariant.hpp"


class ImGUIGraphics {
public:
	ImGUIGraphics() = default;
	ImGUIGraphics(const ImGUIGraphics&) = default;
	~ImGUIGraphics() = default;

	virtual bool Init() const = 0;
	virtual void Shutdown() const = 0;

	virtual void NewFrame() const = 0;
	virtual void RenderDrawData() const = 0;

	virtual void Image(Texture2DVariant texture, glm::vec2 size) const = 0;

};