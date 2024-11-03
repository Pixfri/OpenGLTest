// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <OpenGLTest/pch.hpp>

#include <OpenGLTest/Shader.hpp>

#include <glm/glm.hpp>

#include <vector>

namespace OGLTest {
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UVs;
    };

    enum class TextureType : UInt8 {
        Diffuse,
        Specular
    };

    struct Texture {
        UInt32 Id;
        TextureType Type;
    };

    class Mesh {
    public:
        Mesh(const std::vector<Vertex>& vertices, const std::vector<UInt32>& indices,
             const std::vector<Texture>& textures);
        ~Mesh() = default;

        Mesh(const Mesh&) = delete;
        Mesh(Mesh&&) = delete;

        Mesh& operator=(const Mesh&) = delete;
        Mesh& operator=(Mesh&&) = delete;

        [[nodiscard]] inline std::vector<Vertex> GetVertices() const;
        [[nodiscard]] inline std::vector<UInt32> GetIndices() const;
        [[nodiscard]] inline std::vector<Texture> GetTextures() const;

        void Draw(Shader& shader);

    private:
        std::vector<Vertex> m_Vertices;
        std::vector<UInt32> m_Indices;
        std::vector<Texture> m_Textures;

        GLuint m_VAO, m_VBO, m_EBO;

        void SetupMesh();
    };
}

#include <OpenGLTest/Mesh.inl>
