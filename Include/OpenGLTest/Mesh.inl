// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

namespace OGLTest {
    inline std::vector<Vertex> Mesh::GetVertices() const {
        return m_Vertices;
    }

    inline std::vector<UInt32> Mesh::GetIndices() const {
        return m_Indices;
    }

    inline std::vector<Texture> Mesh::GetTextures() const {
        return m_Textures;
    }
}
