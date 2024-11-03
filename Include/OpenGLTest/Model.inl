// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

namespace OGLTest {
    inline Model::Model(const std::filesystem::path& path) {
        LoadModel(path);
    }
}
