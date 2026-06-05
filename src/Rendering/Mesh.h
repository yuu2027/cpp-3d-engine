#pragma once

#include <vector>
#include <glm/glm.hpp>

#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Renderer.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
};

class Mesh {
public:
    bool Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    void Draw() const;
    void Destroy();

private:
    VertexArray m_vertexArray;
    VertexBuffer m_vertexBuffer;
    IndexBuffer m_indexBuffer;
};