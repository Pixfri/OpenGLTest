// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENS

#include "OpenGLTest/pch.hpp"

#include <OpenGLTest/Shader.hpp>
#include <OpenGLTest/Camera.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb/stb_image.h>

#include <cmath>
#include <chrono>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

OGLTest::Camera g_Camera(glm::vec3(0.0f, 0.0f, 3.0f));
OGLTest::Float32 g_LastX = WINDOW_WIDTH / 2.0f;
OGLTest::Float32 g_LastY = WINDOW_HEIGHT / 2.0f;
bool g_FirstMouse = true;

std::chrono::high_resolution_clock::time_point g_CurrentTime;
OGLTest::Float32 g_DeltaTime = 0.016f;

glm::vec3 g_LightPos(1.2f, 1.0f, 2.0f);

void ProcessInput(GLFWwindow* window, OGLTest::Float32 deltaTime);
OGLTest::UInt32 LoadTexture(const std::string& path);

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW." << '\n';
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window." << '\n';
        return -2;
    }
    glfwMakeContextCurrent(window);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to load OpenGL loader." << '\n';
        return -3;
    }
    
    glfwSetWindowSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        OGLTEST_UNUSED(win);
        glViewport(0, 0, width, height);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, OGLTest::Float64 xPos, OGLTest::Float64 yPos) -> void {
        OGLTEST_UNUSED(win);
        
        if (g_FirstMouse) {
            g_LastX = static_cast<OGLTest::Float32>(xPos);
            g_LastY = static_cast<OGLTest::Float32>(yPos);
            g_FirstMouse = false;
        }
        
        OGLTest::Float32 xOffset = static_cast<OGLTest::Float32>(xPos) - g_LastX;
        OGLTest::Float32 yOffset = g_LastY - static_cast<OGLTest::Float32>(yPos);

        g_LastX = static_cast<OGLTest::Float32>(xPos);
        g_LastY = static_cast<OGLTest::Float32>(yPos);

        g_Camera.ProcessMouseMovement(xOffset, yOffset);
    });

    glfwSetScrollCallback(window, [](GLFWwindow* win, OGLTest::Float64 xOffset, OGLTest::Float64 yOffset) -> void {
        OGLTEST_UNUSED(win);
        OGLTEST_UNUSED(xOffset);

        g_Camera.ProcessMouseScroll(static_cast<OGLTest::Float32>(yOffset));
    });
    
    glEnable(GL_DEPTH_TEST);
    
    OGLTest::Shader lightingShader{"Resources/Shaders/common.vert", "Resources/Shaders/color.frag"};
    OGLTest::Shader lightCubeShader{"Resources/Shaders/common.vert", "Resources/Shaders/light.frag"};
    
    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    // Configure the cube's VAO and VBO
    GLuint VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // UV attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Configure the light's VAO (VBO stays the same, both objects are 3D cubes)
    GLuint lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    // We only need to bind it, no need to set the data inside.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    OGLTest::UInt32 diffuseMap = LoadTexture("Resources/Textures/container.png");
    OGLTest::UInt32 specularMap = LoadTexture("Resources/Textures/container_specular.png");

    lightingShader.Use();
    lightingShader.Set("material.diffuse", 0);
    lightingShader.Set("material.specular", 1);
    lightingShader.Set("material.shininess", 64.f);

    g_CurrentTime = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto oldTime = g_CurrentTime;
        g_CurrentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> timeSpan = (g_CurrentTime - oldTime);
        g_DeltaTime = static_cast<OGLTest::Float32>(timeSpan.count() / 1000.0);
        
        ProcessInput(window, g_DeltaTime);
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lightingShader.Use();
        lightingShader.Set("viewPos", g_Camera.Position);
        
        lightingShader.Set("light.position", g_LightPos);
        lightingShader.Set("light.ambient", 0.2f, 0.2f, 0.2f);
        lightingShader.Set("light.diffuse", 0.5f, 0.5f, 0.5f);
        lightingShader.Set("light.specular", 1.0f, 1.0f, 1.0f);

        glm::mat4 projection = glm::perspective(glm::radians(g_Camera.Fov), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = g_Camera.GetViewMatrix();
        lightingShader.Set("proj", projection);
        lightingShader.Set("view", view);

        // Cube world transform
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.Set("model", model);

        // Bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // Bind specular map
        glActiveTexture(GL_TEXTURE1),
        glBindTexture(GL_TEXTURE_2D, specularMap);

        // Render the cube
        glBindVertexArray(cubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Also draw the light cube
        lightCubeShader.Use();
        lightCubeShader.Set("proj", projection);
        lightCubeShader.Set("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model, g_LightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightCubeShader.Set("model", model);

        glBindVertexArray(lightCubeVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteVertexArrays(1, &cubeVAO);

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}

void ProcessInput(GLFWwindow* window, const OGLTest::Float32 deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
        g_Camera.ProcessKeyboard(OGLTest::CameraMovement::Forward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        g_Camera.ProcessKeyboard(OGLTest::CameraMovement::Backward, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        g_Camera.ProcessKeyboard(OGLTest::CameraMovement::Left, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_SEMICOLON) == GLFW_PRESS) {
        g_Camera.ProcessKeyboard(OGLTest::CameraMovement::Right, deltaTime);
    }
}

OGLTest::UInt32 LoadTexture(const std::string& path) {
    OGLTest::UInt32 textureID;
    glGenTextures(1, &textureID);

    OGLTest::Int32 width, height, nrComponents;
    if (unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0)) {
        GLenum format{};
        if (nrComponents == 1) {
            format = GL_RED;
        }

        if (nrComponents == 3) {
            format = GL_RGB;
        }

        if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cerr << "Texture image failed to load at path: " << path << '\n';
        stbi_image_free(data);
    }

    return textureID;
}
