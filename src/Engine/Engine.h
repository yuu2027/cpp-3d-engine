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
#include "Rendering/Model.h"
#include "Rendering/ModelLoader.h"
#include "Rendering/Light.h"
#include "Debug/DebugGui.h"

#include <glm/mat4x4.hpp>

using namespace std;

struct ModelViewerObject {
	string name;
	Mesh* mesh = nullptr;
	Model* model = nullptr;
	Material* material = nullptr;
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::vec4 materialColor = { 1.0f, 1.0f, 1.0f, 1.0f };
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
	bool InitializeModelViewer();
	bool CreateGroundPlaneMesh();
	bool CreateCubeMesh();

	void ProcessInput();
	void Update();
	void Render();
	void DrawModelViewerObject(ModelViewerObject& object, float aspectRatio);
	glm::mat4 BuildTransform(const ModelViewerObject& object) const;

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
	Texture2D m_groundTexture;
	Texture2D m_modelTexture;

	Mesh m_groundMesh;
	Mesh m_cubeMesh;
	Material m_groundMaterial;
	Material m_cubeMaterial;
	std::vector<ModelViewerObject> m_modelViewerObjects;

	Model m_demoModel;
	ModelLoader m_modelLoader;

	DebugGui m_debugGui;
	LightingSettings m_lighting;

	bool m_isCameraMouseActive = false;
};
