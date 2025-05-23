#include "Resources/Model.hpp"

#include <iostream>
#include "assimp/postprocess.h"

#include "Modules/RenderModule.hpp"

#include "Render/Vertex.hpp"
#include "Render/Mesh.hpp"

#include "Resources/Texture.hpp"

#include "Utilities/Logger.hpp"


static void ReplaceAllOccurrences(std::string& str, const std::string& from, const std::string& to) {
    size_t startPos = 0;
    while ((startPos = str.find(from, startPos)) != std::string::npos) {
        str.replace(startPos, from.length(), to);
        startPos += to.length(); // Move past the last replaced substring
    }
}


Assimp::Importer Model::importer{};


bool Model::Load(const std::string& filePath) {

	const aiScene* scene = importer.ReadFile(
        filePath, 
        aiPostProcessSteps::aiProcess_Triangulate |             // Ensures triangles only
        aiPostProcessSteps::aiProcess_JoinIdenticalVertices |   // Merges duplicate vertices
        aiPostProcessSteps::aiProcess_GenNormals |              // Generates normals if missing
        aiPostProcessSteps::aiProcess_CalcTangentSpace          // Generates tangents and bitangents
    );

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        LOG_ERROR("ERROR::ASSIMP::{}", importer.GetErrorString());
        return false;
    }
    directory = filePath.substr(0, filePath.find_last_of('/'));

    LOG_DEBUG("Loading: {}", filePath);

    ProcessNode(scene->mRootNode, scene);
    return true;
}

void Model::Cleanup() {
    for (Mesh* mesh : meshes) {
        delete mesh;
        mesh = nullptr;
    }

    meshes.clear();
}

void Model::Render(Shader* shader, const glm::mat4x4& transformMatrix) {
    for (Mesh* mesh : meshes) {
        mesh->Render(shader, transformMatrix);
    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {

    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }

    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh* Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        vertices.emplace_back(

            glm::vec3(
                mesh->mVertices[i].x, 
                mesh->mVertices[i].y, 
                mesh->mVertices[i].z
            ),
            mesh->HasTextureCoords(0) ?
                glm::vec2(
                    mesh->mTextureCoords[0][i].x, 
                    mesh->mTextureCoords[0][i].y
                ) :
                glm::vec2(.0f, .0f),
            glm::vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            )

        );
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // process material
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        std::vector<std::shared_ptr<Texture>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        std::vector<std::shared_ptr<Texture>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    LOG_DEBUG("Loaded: {} vertices, {} triangles and {} textures", vertices.size(), indices.size(), textures.size());

    return RenderModule::CreateMesh(vertices, indices, textures);
}

std::vector<std::shared_ptr<Texture>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName) const {
    std::vector<std::shared_ptr<Texture>> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        std::string string = str.C_Str();
        ReplaceAllOccurrences(string, "\\", "/");
        std::shared_ptr<Texture> texture = RenderModule::CreateTexture(directory + '/' + string);
        textures.push_back(texture);
    }
    return textures;
}
