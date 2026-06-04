#include "Rendering/Renderer.h"

#include "Rendering/VertexArray.h"

#include <glad/glad.h>

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::DrawArrays(const VertexArray& vertexArray, int vertexCount) {
    vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}