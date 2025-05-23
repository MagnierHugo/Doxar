#pragma once

#include <map>
#include <string>
#include <memory>
#include <d3d11.h>
#include <wrl/client.h>

// ResourceManager is responsible for loading and caching game resources such as textures.
// It follows the singleton pattern to ensure a unique instance across the engine.
class ResourceManager {
public:
    // Returns the singleton instance of ResourceManager.
    static ResourceManager& GetInstance();

    // Loads a texture from file if not already loaded.
    // device: Pointer to the D3D11 device.
    // path: File path to the texture.
    // Returns a smart pointer to the shader resource view of the texture.
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> LoadTexture(ID3D11Device* device, const std::string& path);

    // Retrieves a previously loaded texture.
    // Returns the shader resource view if found; otherwise, returns nullptr.
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetTexture(const std::string& path);

    // Unloads all resources from memory.
    void UnloadAll();

private:
    // Private constructor for singleton pattern.
    ResourceManager() {}
    ~ResourceManager() {}

    // Disable copy constructor and assignment operator.
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Map storing textures by their file path.
    std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
};
