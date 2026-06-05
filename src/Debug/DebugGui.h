#pragma once

#include "Rendering/Light.h"

#include <vector>

#include <glm/glm.hpp>

struct GLFWwindow;

struct DebugObjectControls {
    const char* name = nullptr;
    glm::vec3* position = nullptr;
    glm::vec3* rotation = nullptr;
    glm::vec3* scale = nullptr;
    glm::vec4* materialColor = nullptr;
};

class DebugGui {
public:
    bool Initialize(GLFWwindow* window);
    void BeginFrame();
    void Draw(
        double fps,
        double deltaTime,
        bool isCameraMouseActive,
        const std::vector<DebugObjectControls>& objects,
        LightingSettings& lighting
    );
    void EndFrame();
    void Shutdown();

private:
    int m_selectedObjectIndex = 0;
};
