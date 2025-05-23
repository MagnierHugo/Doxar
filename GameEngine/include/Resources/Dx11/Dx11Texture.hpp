#pragma once
#ifdef _WINDOWS

#include "Resources/Texture.hpp"


class Dx11Texture : public Texture {

    /*struct TargaHeader {
        unsigned char data1[12];
        unsigned short width;
        unsigned short height;
        unsigned char bpp;
        unsigned char data2;
    };*/

public:
    Dx11Texture() = default;
    Dx11Texture(const Dx11Texture&) = default;
    ~Dx11Texture() = default;

    bool Load(const std::string& filePath) override;
    void Cleanup() override;

    bool Initialize(const char* path) override;
    void Shutdown() override;

    ShaderRessourceViewVariant GetTexture() override;

private:
    /*unsigned char* targaData = nullptr;
    struct ID3D11Texture2D* texture = nullptr;*/
    ID3D11ShaderResourceView* textureView = nullptr;

    bool InitializeTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, const char* path);
    //bool LoadTarga32Bit(const char*);
};

#endif