// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <OpenGLTest/Shader.hpp>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

namespace OGLTest {
    Shader::Shader(const std::filesystem::path& vertexPath, const std::filesystem::path& fragmentPath) {
        // 1. Retrieve the vertex/fragment shader code from the path.
        std::string vertexCode;
        std::string fragmentCode;

        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Ensure ifstream objects can throw exceptions.
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        try {
            // Open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams.
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // Close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // Convert streams into strings.
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        } catch (const std::ifstream::failure& e) {
            std::cerr << "Couldn't read shader file" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        // 2. Compile shaders.
        UInt32 vertex, fragment;
        Int32 success;
        char infoLog[512];

        // Vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        // Print compile errors if any
        glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(vertex, 512, nullptr, infoLog);
            std::cerr << "Failed to compile vertex shader.\nReason:" << infoLog << std::endl;
        }

        // Fragment shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        // Print compile errors if any
        glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);

        if (!success) {
            glGetShaderInfoLog(fragment, 512, nullptr, infoLog);
            std::cerr << "Failed to compile fragment shader.\nReason:" << infoLog << std::endl;
        }

        // Shader program.
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        // Print linking errors if any
        glGetProgramiv(ID, GL_LINK_STATUS, &success);

        if (!success) {
            glGetProgramInfoLog(ID, 512, nullptr, infoLog);
            std::cerr << "Failed to link shader program.\nReason:" << infoLog << std::endl;
        }

        // Delete the shaders as they're linked and no longer necessary.
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::Use() const {
        glUseProgram(ID);
    }
    
    void Shader::Set(const std::string& name, const bool& value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), static_cast<Int32>(value));
    }

    void Shader::Set(const std::string& name, const Int32& value) const {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::Set(const std::string& name, const Float32& value) const {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::Set(const std::string& name, const glm::mat2& value) const {
        glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    }

    void Shader::Set(const std::string& name, const glm::mat3& value) const {
        glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    } 

    void Shader::Set(const std::string& name, const glm::mat4& value) const {
        glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
    } 

    void Shader::Set(const std::string& name, const glm::vec2& value) const {
        glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    } 

    void Shader::Set(const std::string& name, const glm::vec3& value) const {
        glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    } 

    void Shader::Set(const std::string& name, const glm::vec4& value) const {
        glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
    } 

}
