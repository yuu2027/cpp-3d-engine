#include "Rendering/Mesh.h"

bool Mesh::Create(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) {
    if (vertices.empty() || indices.empty()) {
        return false;
    }

    m_vertexArray.Create();
    m_vertexArray.Bind();

    m_vertexBuffer.Create(vertices.data(), vertices.size() * sizeof(Vertex));

    m_vertexArray.SetFloatAttribute(0, 3, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, position)));
    m_vertexArray.SetFloatAttribute(1, 3, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, normal)));
    m_vertexArray.SetFloatAttribute(2, 2, sizeof(Vertex), reinterpret_cast<const void*>(offsetof(Vertex, texCoord)));

    m_indexBuffer.Create(indices.data(), indices.size());

    VertexBuffer::Unbind();
    VertexArray::Unbind();

    return true;
}

void Mesh::Draw() const {
    Renderer::DrawIndexed(m_vertexArray, m_indexBuffer);
}

void Mesh::Destroy() {
    m_indexBuffer.Destroy();
    m_vertexBuffer.Destroy();
    m_vertexArray.Destroy();
}