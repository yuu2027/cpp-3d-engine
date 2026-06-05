#pragma once

#include <glm/glm.hpp>

enum class CameraMovement {
    Forward,
    Backward,
    Left,
    Right
};

class Camera {
public:
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix(float aspectRatio) const;

    void ProcessKeyboard(CameraMovement direction, float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset);

private:
    void UpdateVectors();

    glm::vec3 m_position = { 0.0f, 0.0f, 3.0f };
    glm::vec3 m_front = { 0.0f, 0.0f, -1.0f };
    glm::vec3 m_up = { 0.0f, 1.0f, 0.0f };
    glm::vec3 m_right = { 1.0f, 0.0f, 0.0f };
    glm::vec3 m_worldUp = { 0.0f, 1.0f, 0.0f };

    float m_yaw = -90.0f;
    float m_pitch = 0.0f;
    float m_fov = 45.0f;
    float m_moveSpeed = 3.0f;
    float m_mouseSensitivity = 0.1f;
};