#include "Rendering/Renderer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"

#include <glad/glad.h>

void Renderer::SetViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
}

void Renderer::EnableDepthTest(bool enabled) {
    if (enabled) {
        glEnable(GL_DEPTH_TEST);
    }
    else {
        glDisable(GL_DEPTH_TEST);
    }
}

void Renderer::Clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::DrawArrays(const VertexArray& vertexArray, int vertexCount) {
    vertexArray.Bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Renderer::DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer) {
    vertexArray.Bind();
    indexBuffer.Bind();
    glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
}