#include "Rendering/Renderer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"
#include "Rendering/Camera.h"
#include "Rendering/Shader.h"

#include <glad/glad.h>

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::EnableDepthTest(bool enabled) {
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawArrays(const VertexArray& vertexArray, int vertexCount) {
    vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Renderer::DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer) {
    vertexArray.Bind();
    indexBuffer.Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::DrawMesh(
    const Mesh& mesh,
    const Material& material,
    const Camera& camera,
    const glm::mat4& model,
    float aspectRatio,
    const LightingSettings& lighting
) {
    material.Apply();

    Shader* shader = material.GetShader();
    if (shader == nullptr) {
        return;
    }

    shader->SetMat4("uModel", model);
    shader->SetMat4("uView", camera.GetViewMatrix());
    shader->SetMat4("uProjection", camera.GetProjectionMatrix(aspectRatio));

    shader->SetVec3("uLightDirection", glm::normalize(lighting.directional.direction));
    shader->SetVec3("uLightColor", lighting.directional.color);
    shader->SetFloat("uLightIntensity", lighting.directional.intensity);
    shader->SetVec3("uAmbientColor", lighting.ambient.color);
    shader->SetFloat("uAmbientIntensity", lighting.ambient.intensity);

    mesh.Draw();
}
