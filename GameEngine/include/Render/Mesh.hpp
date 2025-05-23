#pragma once

#include <vector>
#include <memory>
#include "Core/Maths/mat4x4.hpp"

#include "Render/Vertex.hpp"


//class Mesh : public Resource {
//
//public:
//    static Assimp::Importer importer;
//
//    Mesh();
//    ~Mesh();
//
//
//    bool Load(const std::string& filePath) override;
//    void Cleanup() override;
//
//    //PoolVector<struct Vertex> vertices;
//    struct Vertex* vertices = nullptr;
//    unsigned int verticesCount = 0;
//
//    //PoolVector<unsigned int> indices;
//    unsigned int* indices = nullptr;
//    unsigned int indicesCount = 0;
//
//    class ModelClass* modelClass = nullptr;
//    class Shader* shader = nullptr;
//
//    void Render(DeviceContextVariant deviceContext, const glm::mat4x4& transformMatrix);
//private:
//    // returns wether successful
//    bool PrepareVertices(const int verticesCount_);
//    // returns wether successful
//    bool PrepareIndices(const int indicesCount_);
//
//    void Initialize();
//
//    class RenderModule* renderModule = nullptr;
//};


class Mesh {
public:
	Mesh() = default;
	Mesh(const Mesh&) = default;
	Mesh(Mesh&&) = default;
	~Mesh() = default;

	virtual void Render(class Shader* shader, const glm::mat4x4& transformMatrix) = 0;

protected:
	// mesh data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<class Texture>> textures;

	virtual bool SetupMesh() = 0;

};