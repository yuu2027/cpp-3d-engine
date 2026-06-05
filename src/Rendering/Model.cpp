#include "Rendering/Model.h"

#include "Rendering/Renderer.h"

void Model::AddMesh(ModelMesh&& mesh) {
    m_meshes.push_back(std::move(mesh));
}

Texture2D* Model::AddTexture(std::unique_ptr<Texture2D> texture) {
    Texture2D* rawTexture = texture.get();
    m_textures.push_back(std::move(texture));
    return rawTexture;
}

void Model::Draw(const Camera& camera, const glm::mat4& model, float aspectRatio) const {
    for (const ModelMesh& mesh : m_meshes) {
        Renderer::DrawMesh(mesh.mesh, mesh.material, camera, model, aspectRatio);
    }
}

void Model::Destroy() {
    for (ModelMesh& mesh : m_meshes) {
        mesh.mesh.Destroy();
    }

    m_meshes.clear();
    m_textures.clear();
}