// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#pragma once

#include <OpenGLTest/pch.hpp>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OGLTest {
    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum class CameraMovement : UInt8 {
        Forward,
        Backward,
        Left,
        Right
    };

    constexpr Float32 g_Yaw = -90.0f;
    constexpr Float32 g_Pitch = 0.0f;
    constexpr Float32 g_Speed = 2.5f;
    constexpr Float32 g_Sensitivity = 0.1f;
    constexpr Float32 g_Fov = 45.0f;

    // A camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
    class Camera {
    public:
        // Camera attributes
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
        glm::vec3 WorldUp;

        // Euler angles
        Float32 Yaw;
        Float32 Pitch;

        // Camera options
        Float32 MovementSpeed;
        Float32 MouseSensitivity;
        Float32 Fov;

        // constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
               Float32 yaw = g_Yaw, Float32 pitch = g_Pitch);

        // constructor with scalar values
        Camera(Float32 posX, Float32 posY, Float32 posZ, Float32 upX, Float32 upY, Float32 upZ, Float32 yaw, Float32 pitch);
        
        ~Camera() = default;

        Camera(const Camera&) = delete;
        Camera(Camera&&) = delete;

        Camera& operator=(const Camera&) = delete;
        Camera& operator=(Camera&&) = delete;

        // returns the view matrix calculated using Euler Angles and the LookAt Matrix
        inline glm::mat4 GetViewMatrix() const;

        // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(CameraMovement direction, Float32 deltaTime);

        // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(Float32 xOffset, Float32 yOffset, bool constrainPitch = true);

        // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(Float32 yOffset);
        
    private:
        // calculates the front vector from the Camera's (updated) Euler Angles
        void UpdateCameraVectors();
    };
}

#include <OpenGLTest/Camera.inl>
