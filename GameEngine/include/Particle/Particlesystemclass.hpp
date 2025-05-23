#pragma once
#include <d3d11.h>
#include <directxmath.h>
#include "Resources/Dx11/Dx11Texture.hpp"

class ParticleSystemClass {
private:
    struct VertexType
    {
        DirectX::XMFLOAT3 position;
        DirectX::XMFLOAT2 texture;
        DirectX::XMFLOAT4 color;
    };

    struct ParticleType
    {
        float positionX, positionY, positionZ;
        float red, green, blue;
        float velocity;
        bool active;
    };

public:
    ParticleSystemClass();
    ParticleSystemClass(const ParticleSystemClass&);
    ~ParticleSystemClass();

    bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
    void Shutdown();
    bool Frame(float, ID3D11DeviceContext*);
    void Render(ID3D11DeviceContext*);

    ID3D11ShaderResourceView* GetTexture();
    int GetIndexCount();

private:
    bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
    void ReleaseTexture();
    bool InitializeParticleSystem();
    void ShutdownParticleSystem();

    bool InitializeBuffers(ID3D11Device*);
    void ShutdownBuffers();
    void RenderBuffers(ID3D11DeviceContext*);

    void EmitParticles(float);
    void UpdateParticles(float);
    void KillParticles();
    bool UpdateBuffers(ID3D11DeviceContext*);

private:

    Dx11Texture* m_Texture;
    ParticleType* m_particleList;
    VertexType* m_vertices;
    ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
    int m_vertexCount, m_indexCount;

    float m_particleDeviationX, m_particleDeviationY, m_particleDeviationZ;
    float m_particleVelocity, m_particleVelocityVariation;
    float m_particleSize, m_particlesPerSecond;
    int m_maxParticles;

    int m_currentParticleCount;
    float m_accumulatedTime;

};


