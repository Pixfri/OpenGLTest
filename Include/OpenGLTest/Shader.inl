// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OGLTest {
    inline void Shader::Set(const std::string& name, const bool& value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<Int32>(value));
    }

    inline void Shader::Set(const std::string& name, const Int32& value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    inline void Shader::Set(const std::string& name, const Float32& value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    inline void Shader::Set(const std::string& name, const glm::mat2& value) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    inline void Shader::Set(const std::string& name, const glm::mat3& value) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    } 

    inline void Shader::Set(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    } 

    inline void Shader::Set(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }

    inline void Shader::Set(const std::string& name, const Float32 x, const Float32 y) const {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
    }

    inline void Shader::Set(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }

    inline void Shader::Set(const std::string& name, const Float32 x, const Float32 y, const Float32 z) const {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
    }

    inline void Shader::Set(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    }

    inline void Shader::Set(const std::string& name, const Float32 x, const Float32 y, const Float32 z, const Float32 w) const {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
        
    }

}
