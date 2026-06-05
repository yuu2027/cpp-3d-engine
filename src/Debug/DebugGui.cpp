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
    const std::vector<DebugObjectControls>& objects,
    LightingSettings& lighting
) {
    ImGui::Begin("Phase 7 Model Viewer");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Text("DeltaTime: %.4f", deltaTime);
    ImGui::Text("Mode: %s", isCameraMouseActive ? "Camera" : "GUI");
    ImGui::Text("Right mouse hold: Camera control");

    ImGui::Separator();
    ImGui::TextUnformatted("Object");

    if (objects.empty()) {
        ImGui::TextUnformatted("No editable objects.");
    }
    else {
        if (m_selectedObjectIndex < 0 ||
            m_selectedObjectIndex >= static_cast<int>(objects.size())) {
            m_selectedObjectIndex = 0;
        }

        const char* currentName = objects[m_selectedObjectIndex].name;
        if (currentName == nullptr) {
            currentName = "Object";
        }

        if (ImGui::BeginCombo("Selected", currentName)) {
            for (int i = 0; i < static_cast<int>(objects.size()); ++i) {
                const char* objectName = objects[i].name != nullptr ? objects[i].name : "Object";
                const bool isSelected = (m_selectedObjectIndex == i);

                if (ImGui::Selectable(objectName, isSelected)) {
                    m_selectedObjectIndex = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        const DebugObjectControls& selected = objects[m_selectedObjectIndex];

        if (selected.position != nullptr) {
            ImGui::DragFloat3("Position", glm::value_ptr(*selected.position), 0.05f);
        }
        if (selected.rotation != nullptr) {
            ImGui::DragFloat3("Rotation", glm::value_ptr(*selected.rotation), 1.0f);
        }
        if (selected.scale != nullptr) {
            ImGui::DragFloat3("Scale", glm::value_ptr(*selected.scale), 0.05f, 0.1f, 10.0f);
        }
        if (selected.materialColor != nullptr) {
            ImGui::ColorEdit4("Material Color", glm::value_ptr(*selected.materialColor));
        }
    }

    ImGui::Separator();
    ImGui::TextUnformatted("Lighting");

    ImGui::DragFloat3("Light Direction", glm::value_ptr(lighting.directional.direction), 0.05f);
    ImGui::ColorEdit3("Light Color", glm::value_ptr(lighting.directional.color));
    ImGui::SliderFloat("Light Intensity", &lighting.directional.intensity, 0.0f, 3.0f);
    ImGui::ColorEdit3("Ambient Color", glm::value_ptr(lighting.ambient.color));
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
