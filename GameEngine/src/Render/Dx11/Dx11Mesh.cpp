#include "Render/Dx11/Dx11Mesh.hpp"

#include <iostream>
#include <d3d11.h>

#include "Modules/RenderModule.hpp"

#include "Render/APIsVariant.hpp"
#include "Render/Renderer.hpp"
#include "Render/Vertex.hpp"
#include "Render/Shader.hpp"

#include "Resources/Texture.hpp"
#include "Resources/Dx11/Dx11Texture.hpp"

#include "Components/Camera.hpp"


Dx11Mesh::Dx11Mesh(std::vector<struct Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<std::shared_ptr<Texture>> textures_) {
	vertices = vertices_;
	indices = indices_;
	textures = textures_;

    if (!SetupMesh()) {
        throw std::runtime_error("Failed to Setup Mesh");
    }
}

void Dx11Mesh::Render(Shader* shader, const glm::mat4x4& transformMatrix) {
    Renderer* renderer = RenderModule::GetInstance()->renderer;

    DeviceContextVariant deviceContextVariant = renderer->GetDeviceContext();
    ID3D11DeviceContext** ptrDeviceContext = std::get_if<ID3D11DeviceContext*>(&deviceContextVariant);
    if (ptrDeviceContext == nullptr) {
        throw std::runtime_error("Wrong API Device Context Variant");
    }

    ID3D11DeviceContext* deviceContext = *ptrDeviceContext;

    // Set vertex buffer stride and offset.
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    // Set the vertex buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
    deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    ShaderRessourceViewVariant textureView;

    if (textures.size() > 0) { 
        textureView = textures[0]->GetTexture();
    }
    else {
        textureView = RenderModule::CreateTexture("Assets/default_texture.png")->GetTexture();
    }
    
    shader->Render(
        (int)indices.size(),
        transformMatrix, RenderModule::GetInstance()->mainCamera->GetViewMatrix(), renderer->GetProjectionMatrix(),
        textureView
    );
}

bool Dx11Mesh::SetupMesh() {
    DeviceVariant deviceVariant = RenderModule::GetInstance()->renderer->GetDevice();
    ID3D11Device** ptrDevice = std::get_if<ID3D11Device*>(&deviceVariant);
	if (ptrDevice == nullptr) {
		throw std::runtime_error("Wrong API Device Variant");
	}

	ID3D11Device* device = *ptrDevice;

    // Vertex Buffer
    D3D11_BUFFER_DESC vertexBufferDesc{
        .ByteWidth = sizeof(Vertex) * static_cast<UINT>(vertices.size()),
        .Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    D3D11_SUBRESOURCE_DATA vertexData{
        .pSysMem = vertices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    HRESULT result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
    if (FAILED(result)) { return false; }

    // Index Buffer
    D3D11_BUFFER_DESC indexBufferDesc{
        .ByteWidth = sizeof(unsigned int) * static_cast<UINT>(indices.size()),
        .Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT,
        .BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER,
        .CPUAccessFlags = 0,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    D3D11_SUBRESOURCE_DATA indexData{
        .pSysMem = indices.data(),
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };

    result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
    return !FAILED(result);
}