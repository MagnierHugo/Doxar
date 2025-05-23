#ifdef _WINDOWS

#include "Render/Dx11/Dx11Renderer.hpp"

#include <d3d11.h>
#include <dxgi.h>
#include <iostream>

#include "Modules/Window.hpp"
#include "Core/Maths/trigonometric.hpp"
#include "Core/Maths/ext/matrix_clip_space.hpp"


bool Dx11Renderer::Init(Window* window, int width, int height) {
    HWND hwnd = *window;

    DXGI_SWAP_CHAIN_DESC swapChainDesc{
        .BufferDesc{
            .Width = (unsigned int)width,
            .Height = (unsigned int)height,
            .Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM,
        },

        .SampleDesc{
            .Count = 1,
        },

        .BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
        .BufferCount = 1,
        .OutputWindow = hwnd,
        .Windowed = TRUE,
        .SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD,
    };

    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, nullptr, &deviceContext))) {
        return false;
    }

    D3D11_TEXTURE2D_DESC textureDesc = {};
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    textureDesc.SampleDesc.Count = 1;
    textureDesc.Usage = D3D11_USAGE_DEFAULT;
    textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    textureDesc.CPUAccessFlags = 0;

    device->CreateTexture2D(&textureDesc, nullptr, &renderTexture);
    device->CreateRenderTargetView(renderTexture, NULL, &renderTarget);

    ID3D11Texture2D* backBuffer = nullptr;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    device->CreateRenderTargetView(backBuffer, nullptr, &mainRenderTarget);
    backBuffer->Release();

    D3D11_RASTERIZER_DESC rsDesc = {};
    rsDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
    rsDesc.FillMode = D3D11_FILL_SOLID;
    rsDesc.FrontCounterClockwise = false;
    device->CreateRasterizerState(&rsDesc, &rsState);
    deviceContext->RSSetState(rsState);

    // Create depth target
    D3D11_TEXTURE2D_DESC depthDesc{
        .Width = (unsigned int)width,
        .Height = (unsigned int)height,
        .MipLevels = 1,
        .ArraySize = 1,
        .Format = DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT,
        .SampleDesc {
            .Count = 1
        },
        .Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL
    };

    device->CreateTexture2D(&depthDesc, nullptr, &depthTexture);

    D3D11_DEPTH_STENCIL_VIEW_DESC viewDesc{
        .Format = depthDesc.Format,
        .ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DMS,
    };

    device->CreateDepthStencilView(depthTexture, &viewDesc, &depthTarget);

    deviceContext->OMSetRenderTargets(1, &renderTarget, depthTarget);

    D3D11_VIEWPORT viewport {
        .TopLeftX = .0f,
        .TopLeftY = .0f,
        .Width = (FLOAT)width,
        .Height = (FLOAT)height,
        .MinDepth = .0f,
        .MaxDepth = 1.0f
    };
    deviceContext->RSSetViewports(1, &viewport);

    // ?? Initialiser la matrice de projection
    float fieldOfView = glm::radians(45.f);
    float screenAspect = (float)width / (float)height;

    projectionMatrix = glm::perspective(fieldOfView, screenAspect, 0.1f, 1000.0f);

    return true;
}

void Dx11Renderer::Clear(float r, float g, float b) {
    float color[] = { r, g, b, 1.0f };
    deviceContext->ClearRenderTargetView(renderTarget, color);
    deviceContext->ClearRenderTargetView(mainRenderTarget, color);
    deviceContext->ClearDepthStencilView(depthTarget, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH | D3D11_CLEAR_FLAG::D3D11_CLEAR_STENCIL, 1u, 0u);
}

void Dx11Renderer::Present() {
    if (FAILED(swapChain->Present(1u, 0u))) {
        std::cout << "Sad T-T\n";
    }
}

void Dx11Renderer::Shutdown() {
    if (rsState != nullptr) { rsState->Release(); }
    if (depthTarget != nullptr) { depthTarget->Release(); }
    if (depthTexture != nullptr) { depthTexture->Release(); }
    if (swapChain != nullptr) { swapChain->Release(); }
    if (renderTarget != nullptr) { renderTarget->Release(); }
    if (deviceContext != nullptr) { deviceContext->Release(); }
    if (device != nullptr) { device->Release(); }
}

void Dx11Renderer::StartRenderToMainView() {
    deviceContext->OMSetRenderTargets(1, &mainRenderTarget, nullptr);
}

void Dx11Renderer::EndRenderToMainView() {
    deviceContext->OMSetRenderTargets(1, &renderTarget, depthTarget);
}

DeviceVariant Dx11Renderer::GetDevice() const {
    return device;
}

DeviceContextVariant Dx11Renderer::GetDeviceContext() const {
    return deviceContext;
}

Texture2DVariant Dx11Renderer::GetRenderTexture() const {
    return renderTexture;
}

#endif