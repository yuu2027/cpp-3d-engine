#include "Rendering/VertexArray.h"

#include <glad/glad.h>

VertexArray::~VertexArray() {
	Destroy();
}

void VertexArray::Create() {
	Destroy();
	glGenVertexArrays(1, &m_id);
}

void VertexArray::Bind() const {
	glBindVertexArray(m_id);
}

void VertexArray::Unbind() {
	glBindVertexArray(0);
}

void VertexArray::SetFloatAttribute(unsigned int index, int componentCount, int strideBytes, const void* offset) {
	glVertexAttribPointer(index, componentCount, GL_FLOAT, GL_FALSE, strideBytes, offset);
	glEnableVertexAttribArray(index);
}

void VertexArray::Destroy() {
	if (m_id != 0) {
		glDeleteVertexArrays(1, &m_id);
		m_id = 0;
	}
}