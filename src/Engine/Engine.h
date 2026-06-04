#pragma once

#include <memory>
#include <string>

#include "Engine/Time.h"
#include "Platform/Window.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"


using namespace std;

class Engine {
public:
	bool Initialize(const string& title, int width, int height);
	void Run();
	void Shutdown();

private:
	bool InitializeOpenGL(int width, int height);
	bool InitializeDemoTriangle();

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
};