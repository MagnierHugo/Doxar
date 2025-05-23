#pragma once

#include "Render/APIsVariant.hpp"
#include "Resources/Resource.hpp"

class Texture : public Resource {
public:
    Texture() = default;
    Texture(const Texture&) = default;
    ~Texture() = default;

    virtual bool Load(const std::string& filePath) override = 0;
    virtual void Cleanup() override = 0;

    virtual bool Initialize(const char* path) = 0;
    virtual void Shutdown() = 0;

    virtual ShaderRessourceViewVariant GetTexture() = 0;

    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }

protected:
    unsigned int width, height;

};