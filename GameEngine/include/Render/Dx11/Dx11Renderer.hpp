#pragma once
#ifdef _WINDOWS

#include "Render/Renderer.hpp"
#include "Core/Maths/ext/matrix_transform.hpp"


class Dx11Renderer : public Renderer {
public:
    Dx11Renderer() = default;
    Dx11Renderer(const Dx11Renderer&) = default;
    ~Dx11Renderer() = default;

    bool Init(class Window* window, int width, int height) override;
    void Clear(float r, float g, float b) override;
    void Present() override;
    void Shutdown() override;

    void StartRenderToMainView() override;
    void EndRenderToMainView() override;

    // ?? Fonctions à ajouter :
    DeviceVariant GetDevice() const override;
    DeviceContextVariant GetDeviceContext() const override;
    Texture2DVariant GetRenderTexture() const override;

private:
    struct ID3D11Device* device = nullptr;
    struct ID3D11DeviceContext* deviceContext = nullptr;
    struct IDXGISwapChain* swapChain = nullptr;
    struct ID3D11RenderTargetView* mainRenderTarget = nullptr;
    struct ID3D11RasterizerState* rsState = nullptr;

    struct ID3D11Texture2D* depthTexture = nullptr;
    struct ID3D11DepthStencilView* depthTarget = nullptr;

    ID3D11RenderTargetView* renderTarget = nullptr;
    ID3D11Texture2D* renderTexture = nullptr;

};

#endif