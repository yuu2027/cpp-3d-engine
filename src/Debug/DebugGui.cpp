#include "Debug/DebugGui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glm/gtc/type_ptr.hpp>

bool DebugGui::Initialize(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
    return true;
}

void DebugGui::BeginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void DebugGui::Draw(
    double fps,
    double deltaTime,
    bool isCameraMouseActive,
    glm::vec3& position,
    glm::vec3& rotation,
    glm::vec3& scale,
    glm::vec4& materialColor,
    LightingSettings& lighting
) {
    ImGui::Begin("Phase 7 Debug");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("DeltaTime: %.4f", deltaTime);
    ImGui::Text("Mode: %s", isCameraMouseActive ? "Camera" : "GUI");
    ImGui::Text("Right mouse hold: Camera control");

    ImGui::DragFloat3("Position", glm::value_ptr(position), 0.05f);
    ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 1.0f);
    ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.05f, 0.1f, 10.0f);

    ImGui::ColorEdit4("Material Color", glm::value_ptr(materialColor));

    ImGui::DragFloat3("Light Direction", glm::value_ptr(lighting.directional.direction), 0.05f);
    ImGui::ColorEdit3("Light Color", glm::value_ptr(lighting.directional.color));
    ImGui::SliderFloat("Light Intensity", &lighting.directional.intensity, 0.0f, 3.0f);
    ImGui::SliderFloat("Ambient Intensity", &lighting.ambient.intensity, 0.0f, 1.0f);

    ImGui::End();
}

void DebugGui::EndFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void DebugGui::Shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
