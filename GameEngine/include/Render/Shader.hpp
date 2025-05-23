#pragma once

#include "Core/Maths/mat4x4.hpp"
#include "Render/APIsVariant.hpp"


class Shader {
public :
	Shader() = default;
	Shader(const Shader&) = default;
	~Shader() = default;

	//Init & ShutDown
	virtual bool Initialize(class Window* window) = 0;
	virtual void Shutdown() = 0;

	virtual bool Render(
		int indexCount, 
		glm::mat4x4 worldMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix, 
		ShaderRessourceViewVariant texture
	) = 0;

protected:

	struct MatrixBufferType {
		glm::mat4x4 world;
		glm::mat4x4 view;
		glm::mat4x4 projection;
	};

};