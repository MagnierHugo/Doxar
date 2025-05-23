#pragma once

#include "Render/Mesh.hpp"


class Dx11Mesh : public Mesh {
public:
	Dx11Mesh(std::vector<struct Vertex> vertices_, std::vector<unsigned int> indices_, std::vector<std::shared_ptr<Texture>> textures_);
	~Dx11Mesh() = default;

	void Render(Shader* shader, const glm::mat4x4& transformMatrix) override;

private:
	struct ID3D11Buffer* vertexBuffer;
	struct ID3D11Buffer* indexBuffer;

	bool SetupMesh() override;

};