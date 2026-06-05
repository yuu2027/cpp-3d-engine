#include "Rendering/IndexBuffer.h"

#include <glad/glad.h>

IndexBuffer::~IndexBuffer() {
    Destroy();
}

void IndexBuffer::Create(const unsigned int* data, std::size_t count) {
    Destroy();
    m_count = static_cast<int>(count);

    glGenBuffers(1, &m_id);
    Bind();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(count * sizeof(unsigned int)),
        data,
        GL_STATIC_DRAW
    );
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::Destroy() {
    if (m_id != 0) {
        glDeleteBuffers(1, &m_id);
        m_id = 0;
        m_count = 0;
    }
}

int IndexBuffer::GetCount() const {
    return m_count;
}