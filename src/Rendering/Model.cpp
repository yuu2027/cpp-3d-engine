#include "Rendering/Model.h"

#include "Rendering/Renderer.h"

void Model::AddMesh(std::unique_ptr<ModelMesh> mesh) {
    m_meshes.push_back(std::move(mesh));
}

Texture2D* Model::AddTexture(std::unique_ptr<Texture2D> texture) {
    Texture2D* rawTexture = texture.get();
    m_textures.push_back(std::move(texture));
    return rawTexture;
}

void Model::Draw(
    const Camera& camera,
    const glm::mat4& model,
    float aspectRatio,
    const LightingSettings& lighting
) const {
    for (const std::unique_ptr<ModelMesh>& mesh : m_meshes) {
        Renderer::DrawMesh(mesh->mesh, mesh->material, camera, model, aspectRatio, lighting);
    }
}

void Model::SetMaterialColor(const glm::vec4& color) {
    for (const std::unique_ptr<ModelMesh>& mesh : m_meshes) {
        mesh->material.SetColor(color);
    }
}

void Model::Destroy() {
    for (std::unique_ptr<ModelMesh>& mesh : m_meshes) {
        mesh->mesh.Destroy();
    }

    m_meshes.clear();
    m_textures.clear();
}
