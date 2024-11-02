// Copyright (C) 2024 Jean "Pixfri" Letessier 
// This file is part of OpenGL Test.
// For conditions of distribution and use, see copyright notice in LICENSE

#include <OpenGLTest/Camera.hpp>

namespace OGLTest {
    Camera::Camera(glm::vec3 position, glm::vec3 up, Float32 yaw, Float32 pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(g_Speed), MouseSensitivity(g_Sensitivity), Fov(g_Fov) {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    Camera::Camera(Float32 posX, Float32 posY, Float32 posZ, Float32 upX, Float32 upY, Float32 upZ, Float32 yaw, Float32 pitch)
        : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(g_Speed), MouseSensitivity(g_Sensitivity), Fov(g_Fov) {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        UpdateCameraVectors();
    }

    void Camera::ProcessKeyboard(CameraMovement direction, Float32 deltaTime) {
        Float32 velocity = MovementSpeed * deltaTime;

        if (direction == CameraMovement::Forward) {
            Position += Front * velocity;
        }

        if (direction == CameraMovement::Backward) {
            Position -= Front * velocity;
        }

        if (direction == CameraMovement::Right) {
            Position += Right * velocity;
        }

        if (direction == CameraMovement::Left) {
            Position -= Right * velocity;
        }        
    }

    void Camera::ProcessMouseMovement(Float32 xOffset, Float32 yOffset, bool constrainPitch) {
        xOffset *= MouseSensitivity;
        yOffset *= MouseSensitivity;

        Yaw += xOffset;
        Pitch += yOffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch) {
            if (Pitch > 89.0f) {
                Pitch = 89.0f;
            }

            if (Pitch < -89.0f) {
                Pitch = -89.0f;
            }
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        UpdateCameraVectors();
    }

    void Camera::ProcessMouseScroll(const Float32 yOffset) {
        Fov -= yOffset;
        if (Fov < 1.0f) {
            Fov = 1.0f;
        }

        if (Fov > 45.0f) {
            Fov = 45.0f;
        }
    }

    void Camera::UpdateCameraVectors() {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);

        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up = glm::normalize(glm::cross(Right, Front));
    }

}
