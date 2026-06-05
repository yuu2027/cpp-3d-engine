#pragma once

#include "Rendering/Light.h"

struct GLFWwindow;

class DebugGui {
public:
    bool Initialize(GLFWwindow* window);
    void BeginFrame();
    void Draw(
        double fps,
        double deltaTime,
        bool isCameraMouseActive,
        glm::vec3& position,
        glm::vec3& rotation,
        glm::vec3& scale,
        glm::vec4& materialColor,
        LightingSettings& lighting
    );
    void EndFrame();
    void Shutdown();
};
