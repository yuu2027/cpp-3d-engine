#pragma once

#include <glm/mat4x4.hpp>

using namespace std;

class VertexArray;
class IndexBuffer;
class Mesh;
class Material;
class Camera;

class Renderer {
public:
    static void SetViewport(int x, int y, int width, int height);
    static void SetClearColor(float r, float g, float b, float a);
    static void EnableDepthTest(bool enabled);
    static void Clear();
    static void DrawArrays(const VertexArray& vertexArray, int vertexCount);
    static void DrawIndexed(const VertexArray& vertexArray, const IndexBuffer& indexBuffer);
    static void DrawMesh(const Mesh& mesh, const Material& material, const Camera& camera, const glm::mat4& model, float aspectRatio);
};