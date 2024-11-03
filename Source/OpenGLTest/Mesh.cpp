// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <OpenGLTest/Mesh.hpp>

namespace OGLTest {
    Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<UInt32>& indices,
               const std::vector<Texture>& textures)
        : m_Vertices(vertices), m_Indices(indices), m_Textures(textures) {
        SetupMesh();
    }

    void Mesh::SetupMesh() {
        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);

        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(UInt32), m_Indices.data(), GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // Vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // Vertex UVs
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UVs));

        glBindVertexArray(0);
    }

    void Mesh::Draw(Shader& shader) {
        UInt32 diffuseNr = 1;
        UInt32 specularNr = 1;
        UInt32 shininessNr = 1;

        for (UInt32 i = 0; i < m_Textures.size(); i++) {
            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name;
            switch (m_Textures[i].Type) {
            case TextureType::Diffuse:
                {
                    number = std::to_string(diffuseNr++);
                    name = "texture_diffuse";
                    break;
                }
            case TextureType::Specular:
                {
                    number = std::to_string(specularNr++);
                    name = "texture_specular";
                    break;
                }
            case TextureType::Shininess:
                {
                    number = std::to_string(shininessNr++);
                    name = "texture_sininess";
                    break;
                }
            }

            shader.Set(("material." + name + number), static_cast<Int32>(i));
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].Id);
        }
        glActiveTexture(GL_TEXTURE0);

        // Draw mesh
        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

}
