// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <OpenGLTest/pch.hpp>

#include <filesystem>
#include <string>

namespace OGLTest {
    class Shader {
    public:
        UInt32 ID;

        Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath);
        ~Shader() = default;
        
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        void Use() const;

        void Set(const std::string& name, const bool& value) const;
        void Set(const std::string& name, const Int32& value) const;
        void Set(const std::string& name, const Float32& value) const;
        
    private:
    };
}

#include <OpenGLTest/Shader.inl>
