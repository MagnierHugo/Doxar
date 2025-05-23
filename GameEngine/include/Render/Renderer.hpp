#pragma once

#include "Core/Maths/mat4x4.hpp"
#include "Core/Maths/ext/matrix_transform.hpp"
#include "Render/APIsVariant.hpp"

/**
 * \brief Abstract Base class Renderer to allow multiple graphics RenderingAPI
 */
class Renderer {
public:
	/**
	 * \brief Default Ctor
	 */
	Renderer() = default;
	/**
	 * \brief Default Dtor
	 */
	~Renderer() = default;

	/**
	 * \brief Initialize the Renderer
	 * \param window -> pointer towards the Window module
	 * \param w -> the width of the renderer
	 * \param h -> the height of the renderer
	 */
	virtual bool Init(class Window* window, int w, int h) = 0;
	/**
	 * \brief Clear the renderer
	 * \param r -> Amount of Red in the clearing color
	 * \param g -> Amount of Green in the clearing color
	 * \param b -> Amount of Blue in the clearing color
	 */
	virtual void Clear(float r, float g, float b) = 0;
	/**
	 * \brief Draw the buffer on screen
	 */
	virtual void Present() = 0;
	/**
	 * \brief Close the renderer
	 */
	virtual void Shutdown() = 0;

	/**
	 * \brief Switch the Render Target View to render on the window instead of a texture
	 */
	virtual void StartRenderToMainView() = 0;
	/**
	 * \brief Switch back to render on the texture
	 */
	virtual void EndRenderToMainView() = 0;

	virtual DeviceVariant GetDevice() const = 0;
	virtual DeviceContextVariant GetDeviceContext() const = 0;
	virtual Texture2DVariant GetRenderTexture() const = 0;
	glm::mat4x4 GetProjectionMatrix() const { return projectionMatrix; }
	glm::mat4x4 GetWorldMatrix() const { return worldMatrix; }


protected:
	glm::mat4x4 projectionMatrix = glm::identity<glm::mat4x4>();
	glm::mat4x4 worldMatrix = glm::identity<glm::mat4x4>();

};