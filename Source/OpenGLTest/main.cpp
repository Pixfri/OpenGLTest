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

void ProcessInput(GLFWwindow* window, OGLTest::Float32 deltaTime);

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
    
    float vertices[] = {
        // Positions          // UVs
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    GLuint VBO, VAO;
    glGenBuffers(1, &VBO);
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    OGLTest::Shader shader{"Resources/Shaders/main.vert", "Resources/Shaders/main.frag"};

    OGLTest::UInt32 texture0, texture1;
    glGenTextures(1, &texture0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    OGLTest::Int32 width, height, nrChannels;
    unsigned char* data = stbi_load("Resources/Textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << '\n';
    }

    glGenTextures(1, &texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    data = stbi_load("Resources/Textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture" << '\n';
    }    
    
    stbi_image_free(data);

    shader.Use();
    shader.Set("texture0", 0);
    shader.Set("texture1", 1);
    
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f), 
        glm::vec3( 2.0f,  5.0f, -15.0f), 
        glm::vec3(-1.5f, -2.2f, -2.5f),  
        glm::vec3(-3.8f, -2.0f, -12.3f),  
        glm::vec3( 2.4f, -0.4f, -3.5f),  
        glm::vec3(-1.7f,  3.0f, -7.5f),  
        glm::vec3( 1.3f, -2.0f, -2.5f),  
        glm::vec3( 1.5f,  2.0f, -2.5f), 
        glm::vec3( 1.5f,  0.2f, -1.5f), 
        glm::vec3(-1.3f,  1.0f, -1.5f)  
    };

    g_CurrentTime = std::chrono::high_resolution_clock::now();
    
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        auto oldTime = g_CurrentTime;
        g_CurrentTime = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::milli> timeSpan = (g_CurrentTime - oldTime);
        g_DeltaTime = static_cast<OGLTest::Float32>(timeSpan.count() / 1000.0);
        
        ProcessInput(window, g_DeltaTime);
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shader.Use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glm::mat4 projection = glm::perspective(glm::radians(g_Camera.Fov), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
        shader.Set("proj", projection);

        shader.Set("view", g_Camera.GetViewMatrix());
        
        glBindVertexArray(VAO);
        for (OGLTest::UInt32 i = 0; i < 10; i++) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            OGLTest::Float32 angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.Set("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(0);

        glfwSwapBuffers(window);
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

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
