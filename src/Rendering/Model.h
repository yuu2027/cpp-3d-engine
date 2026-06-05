#pragma once

#include <memory>
#include <vector>

#include <glm/mat4x4.hpp>

#include "Rendering/Material.h"
#include "Rendering/Mesh.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Light.h"

class Camera;

struct ModelMesh {
    Mesh mesh;
    Material material;
};

class Model {
public:
    void AddMesh(std::unique_ptr<ModelMesh> mesh);
    Texture2D* AddTexture(std::unique_ptr<Texture2D> texture);

    void Draw(const Camera& camera, const glm::mat4& model, float aspectRatio, const LightingSettings& lighting) const;
    void SetMaterialColor(const glm::vec4& color);
    void Destroy();

private:
    std::vector<std::unique_ptr<ModelMesh>> m_meshes;
    std::vector<std::unique_ptr<Texture2D>> m_textures;
};