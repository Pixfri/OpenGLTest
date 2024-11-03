// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <OpenGLTest/Model.hpp>
#include <OpenGLTest/TextureUtils.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace OGLTest {
    void Model::Draw(Shader& shader) {
        for (auto& mesh : m_Meshes) {
            mesh.Draw(shader);
        }
    }

    void Model::LoadModel(const std::filesystem::path& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Couldn't import model at path: " << path << "\nError: " << importer.GetErrorString() << '\n';
            return;
        }
        m_Directory = path.string().substr(0, path.string().find_last_of('/'));

        ProcessNode(scene->mRootNode, scene);
    }

    void Model::ProcessNode(const aiNode* node, const aiScene* scene) {
        // process all the node's meshes (if any)
        for (UInt32 i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_Meshes.push_back(ProcessMesh(mesh, scene));
        }

        // Then do the same for each of its children
        for (UInt32 i = 0; i < node->mNumChildren; i++) {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
        std::vector<Vertex> vertices;
        std::vector<UInt32> indices;
        std::vector<Texture> textures;

        for (UInt32 i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            // Check if the mesh has UVs
            if (mesh->mTextureCoords[0]) {
                glm::vec2 vec;
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.UVs = vec;
            } else {
                vertex.UVs = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }

        for (UInt32 i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];

            for (UInt32 j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
            std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE,
                                                                    TextureType::Diffuse);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR,
                                                                     TextureType::Specular);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            std::vector<Texture> shininessMaps = LoadMaterialTextures(material, aiTextureType_SHININESS,
                                                                      TextureType::Shininess);
            textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());
        }

        return Mesh{vertices, indices, textures};
    }

    std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType) {
        std::vector<Texture> textures;

        for (UInt32 i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;

            for (UInt64 j = 0; j < m_LoadedTextures.size(); j++) {
                if (std::strcmp(m_LoadedTextures[j].Path.data(), str.C_Str()) == 0) {
                    textures.push_back(m_LoadedTextures[j]);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                Texture texture;
                texture.Id = LoadTextureFromFile(str.C_Str(), m_Directory);
                texture.Type = textureType;
                texture.Path = str.C_Str();
                textures.push_back(texture);
                m_LoadedTextures.push_back(texture);
            }
        }

        return textures;
    }
}
