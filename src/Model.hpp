#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


constexpr TextureType ToTextureType(aiTextureType type)
{
    switch (type)
    {
    case aiTextureType_DIFFUSE:
        return TextureType::Diffuse;

    case aiTextureType_SPECULAR:
        return TextureType::Specular;

    case aiTextureType_NORMALS:
        return TextureType::Normal;

    case aiTextureType_HEIGHT:
        return TextureType::Height;

    default:
        return TextureType::Unknown;
    }
}



class Model
{
public:
    Model(std::string const& path, TextureManager & textureManager)
    {
        LoadModel(path, textureManager);
    }

    void Draw(Shader & shader)
    {
        for (auto& mesh : meshes)
            mesh.Draw(shader);
    }

private:
    void LoadModel(std::string const& path, TextureManager& textureManager)
    {
        auto importer = Assimp::Importer();
        auto scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        ProcessNode(scene->mRootNode, scene, textureManager);
    }

    void ProcessNode(aiNode* node, const aiScene* scene, TextureManager& textureManager)
    {
        for (auto i = 0u; i < node->mNumMeshes; i++)
        {
            auto* mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(ProcessMesh(mesh, scene, textureManager));
        }

        for (auto i = 0u; i < node->mNumChildren; i++)
            ProcessNode(node->mChildren[i], scene, textureManager);
    }

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene, TextureManager& textureManager)
    {
        std::vector<Vertex> vertices;
        vertices.reserve(mesh->mNumVertices);

        for (auto i = 0u; i < mesh->mNumVertices; i++)
        {
            vertices.push_back({
                glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z),
                glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z),
                mesh->mTextureCoords[0]
                    ? glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0.0f)
            });
        }

        std::vector<unsigned int> indices;
        indices.reserve(3u * mesh->mNumFaces);

        for (auto i = 0u; i < mesh->mNumFaces; i++)
        {
            auto face = mesh->mFaces[i];
            for (auto j = 0u; j < face.mNumIndices; j++)
                indices.emplace_back(face.mIndices[j]);
        }

        std::vector<TexturePtr> textures;

        if (mesh->mMaterialIndex >= 0)
        {
            auto* material = scene->mMaterials[mesh->mMaterialIndex];

            auto textureCount =
                material->GetTextureCount(aiTextureType_DIFFUSE)
                + material->GetTextureCount(aiTextureType_SPECULAR);

            textures.reserve(textureCount);

            auto diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, textureManager);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            auto specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, textureManager);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(vertices, indices, textures);
    }

    std::vector<TexturePtr> LoadMaterialTextures(
        aiMaterial* material,
        aiTextureType type,
        TextureManager& textureManager)
    {
        std::vector<TexturePtr> textures;
        textures.reserve(material->GetTextureCount(type));

        for (auto i = 0u; i < material->GetTextureCount(type); i++)
        {
            aiString path;
            material->GetTexture(type, i, &path);

            auto fullPath = directory + '/' + path.C_Str();

            textures.emplace_back(textureManager.LoadTexture(fullPath, ToTextureType(type)));
        }

        return textures;
    }


private:
    std::vector<Mesh> meshes; // ToDo: Ptrs
    std::string directory;
};