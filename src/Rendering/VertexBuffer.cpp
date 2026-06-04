#include "Rendering/VertexBuffer.h"

#include <glad/glad.h>

VertexBuffer::~VertexBuffer() {
	Destroy();
}

void VertexBuffer::Create(const void* data, size_t sizeBytes) {
	Destroy();

	glGenBuffers(1, &m_id);
	Bind();

	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeBytes), data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}

void VertexBuffer::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Destroy() {
	if (m_id != 0) {
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
}