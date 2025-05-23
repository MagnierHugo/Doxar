#include "Modules/ResourcesManager.hpp"
#include <stdexcept>

#include <Utilities/WICTextureLoader.h>

ResourceManager& ResourceManager::GetInstance() {
    static ResourceManager instance;
    return instance;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourceManager::LoadTexture(ID3D11Device* device, const std::string& path) {
    // Check if the texture is already loaded.
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second;
    }

    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;

    // Load the texture from file using DirectXTK's WICTextureLoader.
    // The second parameter (ID3D11DeviceContext*) is set to nullptr since it is optional here.
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, nullptr, nullptr, &texture);
    if (FAILED(hr)) {
        // Error handling: throw an exception or handle the error appropriately.
        throw std::runtime_error("Failed to load texture: " + path);
    }

    // Cache the loaded texture.
    m_textures[path] = texture;
    return texture;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ResourceManager::GetTexture(const std::string& path) {
    auto it = m_textures.find(path);
    if (it != m_textures.end()) {
        return it->second;
    }
    return nullptr;
}

void ResourceManager::UnloadAll() {
    // Clear all cached textures.
    m_textures.clear();
}
