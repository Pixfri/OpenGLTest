// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <OpenGLTest/pch.hpp>

namespace OGLTest {
    inline UInt32 LoadTextureFromFile(const char* path, const std::string& directory, bool gamma = false);
}

#include <OpenGLTest/TextureUtils.inl>
