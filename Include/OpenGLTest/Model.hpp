// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once


#include <OpenGLTest/pch.hpp>

#include <OpenGLTest/Shader.hpp>
#include <OpenGLTest/Mesh.hpp>

#include <assimp/scene.h>

#include <filesystem>

namespace OGLTest {
    class Model {
    public:
        inline explicit Model(const std::filesystem::path& path);
        ~Model() = default;

        void Draw(Shader& shader);

    private:
        std::vector<Mesh> m_Meshes;
        std::string m_Directory;
        std::vector<Texture> m_LoadedTextures;

        void LoadModel(const std::filesystem::path& path);
        void ProcessNode(const aiNode* node, const aiScene* scene);
        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
    };
}

#include <OpenGLTest/Model.inl>
