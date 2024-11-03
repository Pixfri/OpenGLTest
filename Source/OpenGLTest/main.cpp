// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENS

#include "OpenGLTest/pch.hpp"

#include <OpenGLTest/Shader.hpp>
#include <OpenGLTest/Camera.hpp>
#include <OpenGLTest/Model.hpp>

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
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Test", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create window." << '\n';
        return -2;
    }
    glfwMakeContextCurrent(window);

    GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* videoMode = glfwGetVideoMode(primaryMonitor);

    const OGLTest::Int32 windowLeft = videoMode->width / 2 - WINDOW_WIDTH / 2;
    const OGLTest::Int32 windowTop = videoMode->height / 2 - WINDOW_HEIGHT / 2;
    glfwSetWindowPos(window, windowLeft, windowTop);

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

    stbi_set_flip_vertically_on_load(true);

    OGLTest::Shader shader{"Resources/Shaders/common.vert", "Resources/Shaders/pointlight.frag"};

    OGLTest::Model model{"Resources/Models/backpack/backpack.obj"};

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

        shader.Use();
        
        glm::mat4 projection = glm::perspective(glm::radians(g_Camera.Fov),
                                                static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT),
                                                0.1f, 100.0f);        
        glm::mat4 view = g_Camera.GetViewMatrix();
        shader.Set("proj", projection);
        shader.Set("view", view);

        // render the loaded model
        glm::mat4 modelMat = glm::mat4(1.0f);
        modelMat = glm::translate(modelMat, glm::vec3(0.0f, 0.0f, 0.0f)); 
        modelMat = glm::scale(modelMat, glm::vec3(1.0f, 1.0f, 1.0f));
        shader.Set("model", modelMat);

        shader.Set("light.position", -0.5f, 1.0f, 5.0f);
        shader.Set("light.ambient", 0.05f, 0.025f, 0.025f);
        shader.Set("light.diffuse", 0.75f, 0.5f, 0.25f);
        shader.Set("light.specular", 1.5f, 1.5f, 1.5f);
        shader.Set("light.constant", 1.0f);
        shader.Set("light.linear", 0.09f);
        shader.Set("light.quadratic", 0.032f);
        
        model.Draw(shader);

        glfwSwapBuffers(window);
    }

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
