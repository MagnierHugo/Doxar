#pragma once

#include <vector>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "Core/Maths/mat4x4.hpp"

#include "Resources/Resource.hpp"


namespace std {
	template<typename T> class shared_ptr;
}


class Model : public Resource {
public:
	Model() = default;
	~Model() = default;

	bool Load(const std::string& filePath) override;
	void Cleanup() override;

	void Render(class Shader* shader, const glm::mat4x4& transformMatrix);

	std::vector<class Mesh*> GetMeshes() const { return meshes; }

private:
	static Assimp::Importer importer;
	std::vector<Mesh*> meshes;
	std::string directory;
	
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<std::shared_ptr<class Texture>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) const;

};