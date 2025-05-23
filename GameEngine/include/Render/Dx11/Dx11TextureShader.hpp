#pragma once
#ifdef _WINDOWS

#include <d3d11.h>

#include "Render/Shader.hpp"


class Dx11TextureShader : public Shader {
public:
	Dx11TextureShader() = default;
	Dx11TextureShader(const Dx11TextureShader&) = default;
	~Dx11TextureShader() = default;

	//Init & ShutDown
	bool Initialize(class Window* window) override;
	void Shutdown() override;

	bool Render(
		int indexCount,
		glm::mat4x4 worldMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix,
		ShaderRessourceViewVariant texture
	) override;

private:
	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader = nullptr;
	ID3D11InputLayout* inputLayout = nullptr;
	ID3D11Buffer* matrixBuffer = nullptr;
	ID3D11SamplerState* sampleState = nullptr;

	bool InitializeShader(ID3D11Device* device, const HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob* errorMessage, const HWND hwnd, WCHAR* shaderFilename);

	bool SetShaderParameters(
		ID3D11DeviceContext* deviceContext,
		glm::mat4x4 worldMatrix, glm::mat4x4 viewMatrix, glm::mat4x4 projectionMatrix,
		ID3D11ShaderResourceView* texture
	);

	void RenderShader(ID3D11DeviceContext* deviceContext, const int indicesCount);

};

#endif