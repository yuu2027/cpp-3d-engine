#include "Rendering/Camera.h"

#include <algorithm>
#include <cmath>

#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

glm::mat4 Camera::GetProjectionMatrix(float aspectRatio) const {
    return glm::perspective(glm::radians(m_fov), aspectRatio, 0.1f, 100.0f);
}

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    const float velocity = m_moveSpeed * deltaTime;

    if (direction == CameraMovement::Forward) {
        m_position += m_front * velocity;
    }
    if (direction == CameraMovement::Backward) {
        m_position -= m_front * velocity;
    }
    if (direction == CameraMovement::Left) {
        m_position -= m_right * velocity;
    }
    if (direction == CameraMovement::Right) {
        m_position += m_right * velocity;
    }
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset) {
    m_yaw += xOffset * m_mouseSensitivity;
    m_pitch += yOffset * m_mouseSensitivity;
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    UpdateVectors();
}

void Camera::UpdateVectors() {
    glm::vec3 front;
    front.x = std::cos(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));
    front.y = std::sin(glm::radians(m_pitch));
    front.z = std::sin(glm::radians(m_yaw)) * std::cos(glm::radians(m_pitch));

    m_front = glm::normalize(front);
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}