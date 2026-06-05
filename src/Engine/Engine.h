#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Engine/Time.h"
#include "Platform/Window.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Camera.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/Texture2D.h"
#include "Rendering/Mesh.h"
#include "Rendering/Material.h"

#include <glm/mat4x4.hpp>

using namespace std;

struct RenderObject {
	Mesh* mesh = nullptr;
	Material* material = nullptr;
	glm::mat4 model = glm::mat4(1.0f);
};

class Engine {
public:
	bool Initialize(const string& title, int width, int height);
	void Run();
	void Shutdown();

private:
	bool InitializeOpenGL(int width, int height);
	bool InitializeDemoTriangle();
	bool InitializeDemoCube();

	void ProcessInput();
	void Update();
	void Render();

	unique_ptr<Window> m_window;
	Time m_time;
	bool m_isRunning = false;

	Shader m_basicShader;
	VertexArray m_triangleVertexArray;
	VertexBuffer m_triangleVertexBuffer;
	int m_triangleVertexCount = 0;

	Camera m_camera;

	VertexArray m_cubeVertexArray;
	VertexBuffer m_cubeVertexBuffer;
	IndexBuffer m_cubeIndexBuffer;

	bool m_isFirstMouseMove = true;
	double m_lastMouseX = 0.0;
	double m_lastMouseY = 0.0;

	Shader m_textureShader;
	Texture2D m_cubeTexture;

	Mesh m_cubeMesh;
	Material m_redMaterial;
	Material m_greenMaterial;
	Material m_blueMaterial;
	std::vector<RenderObject> m_renderObjects;
};