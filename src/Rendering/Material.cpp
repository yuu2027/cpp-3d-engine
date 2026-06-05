#include "Rendering/Material.h"

#include "Rendering/Shader.h"
#include "Rendering/Texture2D.h"

void Material::SetShader(Shader* shader) {
    m_shader = shader;
}

void Material::SetTexture(Texture2D* texture) {
    m_texture = texture;
}

void Material::SetColor(const glm::vec4& color) {
    m_color = color;
}

Shader* Material::GetShader() const {
    return m_shader;
}

void Material::Apply() const {
    if (m_shader == nullptr) {
        return;
    }

    m_shader->Use();
    m_shader->SetFloat4("uColor", m_color.r, m_color.g, m_color.b, m_color.a);

    if (m_texture != nullptr) {
        m_texture->Bind(0);
        m_shader->SetInt("uTexture", 0);
    }
}