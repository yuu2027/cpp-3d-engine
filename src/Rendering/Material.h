#pragma once

#include <glm/glm.hpp>

class Shader;
class Texture2D;

class Material {
public:
    void SetShader(Shader* shader);
    void SetTexture(Texture2D* texture);
    void SetColor(const glm::vec4& color);

    Shader* GetShader() const;
    void Apply() const;

private:
    Shader* m_shader = nullptr;
    Texture2D* m_texture = nullptr;
    glm::vec4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
};