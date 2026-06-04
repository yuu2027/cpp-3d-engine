#pragma once

using namespace std;

class VertexArray;

class Renderer {
public:
    static void SetViewport(int x, int y, int width, int height);
    static void SetClearColor(float r, float g, float b, float a);
    static void Clear();
    static void DrawArrays(const VertexArray& vertexArray, int vertexCount);
};