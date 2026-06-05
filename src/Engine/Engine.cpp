#include "Engine/Engine.h"

#include <array>
# include <iostream>
#include <string>
#include <vector>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

#include "Rendering/Renderer.h"

#ifndef CPP3D_ASSET_DIR
#define CPP3D_ASSET_DIR "assets"
#endif

namespace {
	string GetAssetPath(const string& relativePath) {
		return string(CPP3D_ASSET_DIR) + "/" + relativePath;
	}
}

bool Engine::Initialize(const string& title, int width, int height) {
	m_window = make_unique<Window>(); // Windowを作成し、その所有権をm_windowに持たせる

	if (!m_window->Create(title, width, height)) {
		cerr << "Failed to create window.\n";
		return false;
	}

	if (!InitializeOpenGL(width, height)) {
		return false;
	}

	if (!InitializeModelViewer()) {
		return false;
	}

	Renderer::SetClearColor(0.02f, 0.02f, 0.025f, 1.0f);
	Renderer::EnableDepthTest(true);

	if (!m_debugGui.Initialize(m_window->GetNativeHandle())) {
		return false;
	}

	glfwSetInputMode(m_window->GetNativeHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	m_isRunning = true;

	return true;
}

bool Engine::InitializeOpenGL(int width, int height) {
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
		cerr << "Failed to initialize GLAD.\n";
		return false;
	}

	Renderer::SetViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(m_window->GetNativeHandle(), [](GLFWwindow*, int framebufferWidth, int framebufferHeight) {Renderer::SetViewport(0, 0, framebufferWidth, framebufferHeight); });
	
	return true;
}

bool Engine::InitializeModelViewer() {
	if (!m_textureShader.LoadFromFiles(
		GetAssetPath("shaders/lit_textured.vert"),
		GetAssetPath("shaders/lit_textured.frag"))) {
		return false;
	}

	if (!m_groundTexture.LoadFromFile(GetAssetPath("textures/stone_tiles_albedo_512.png"))) {
		return false;
	}

	if (!m_cubeTexture.LoadFromFile(GetAssetPath("textures/phase6_cube_uv_checker.png"))) {
		return false;
	}

	if (!m_modelTexture.LoadFromFile(GetAssetPath("textures/metal_panel_albedo_512.png"))) {
		return false;
	}

	if (!CreateGroundPlaneMesh()) {
		return false;
	}

	if (!CreateCubeMesh()) {
		return false;
	}

	if (!m_modelLoader.LoadObj(
		GetAssetPath("models/phase6_unit_cube.obj"),
		m_demoModel,
		&m_textureShader,
		&m_modelTexture)) {
		return false;
	}

	m_groundMaterial.SetShader(&m_textureShader);
	m_groundMaterial.SetTexture(&m_groundTexture);
	m_groundMaterial.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_cubeMaterial.SetShader(&m_textureShader);
	m_cubeMaterial.SetTexture(&m_cubeTexture);
	m_cubeMaterial.SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	m_modelViewerObjects = {
		{
			"Ground Plane",
			&m_groundMesh,
			nullptr,
			&m_groundMaterial,
			{ 0.0f, -1.0f, -3.0f },
			{ 0.0f, 0.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 0.9f, 0.9f, 0.9f, 1.0f }
		},
		{
			"Textured Cube",
			&m_cubeMesh,
			nullptr,
			&m_cubeMaterial,
			{ -1.75f, 0.0f, -3.0f },
			{ 0.0f, 25.0f, 0.0f },
			{ 1.0f, 1.0f, 1.0f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		},
		{
			"OBJ Model",
			nullptr,
			&m_demoModel,
			nullptr,
			{ 1.75f, 0.0f, -3.0f },
			{ 0.0f, -25.0f, 0.0f },
			{ 1.25f, 1.25f, 1.25f },
			{ 1.0f, 1.0f, 1.0f, 1.0f }
		}
	};

	return true;
}

bool Engine::CreateGroundPlaneMesh() {
	const std::vector<Vertex> vertices = {
		{{ -5.0f, 0.0f, -5.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
		{{  5.0f, 0.0f, -5.0f }, { 0.0f, 1.0f, 0.0f }, { 4.0f, 0.0f }},
		{{  5.0f, 0.0f,  5.0f }, { 0.0f, 1.0f, 0.0f }, { 4.0f, 4.0f }},
		{{ -5.0f, 0.0f,  5.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 4.0f }},
	};

	const std::vector<unsigned int> indices = {
		0, 1, 2,
		2, 3, 0,
	};

	return m_groundMesh.Create(vertices, indices);
}

bool Engine::CreateCubeMesh() {
	const std::vector<Vertex> vertices = {
		{{ -0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }},
		{{  0.5f, -0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }},
		{{  0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }},
		{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }},

		{{  0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f }},
		{{ -0.5f, -0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f }},
		{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f }},
		{{  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f }},

		{{ -0.5f, -0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
		{{ -0.5f, -0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},
		{{ -0.5f,  0.5f,  0.5f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
		{{ -0.5f,  0.5f, -0.5f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},

		{{  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f }},
		{{  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f }},
		{{  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f }},
		{{  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f }},

		{{ -0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f }},
		{{  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f }},
		{{  0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 1.0f }},
		{{ -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f }},

		{{ -0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f }},
		{{  0.5f, -0.5f, -0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f }},
		{{  0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 1.0f }},
		{{ -0.5f, -0.5f,  0.5f }, { 0.0f, -1.0f, 0.0f }, { 0.0f, 1.0f }},
	};

	const std::vector<unsigned int> indices = {
		 0,  1,  2,  2,  3,  0,
		 4,  5,  6,  6,  7,  4,
		 8,  9, 10, 10, 11,  8,
		12, 13, 14, 14, 15, 12,
		16, 17, 18, 18, 19, 16,
		20, 21, 22, 22, 23, 20,
	};

	return m_cubeMesh.Create(vertices, indices);
}

bool Engine::InitializeDemoTriangle() {
	if (!m_basicShader.LoadFromFiles(GetAssetPath("shaders/basic_color.vert"), GetAssetPath("shaders/basic_color.frag"))) {
		return false;
	}

	const float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f
	};

	m_triangleVertexCount = 3;

	m_triangleVertexArray.Create();
	m_triangleVertexArray.Bind();

	m_triangleVertexBuffer.Create(vertices, sizeof(vertices));
	m_triangleVertexArray.SetFloatAttribute(0, 3, 3 * sizeof(float), nullptr);

	VertexBuffer::Unbind();
	VertexArray::Unbind();

	return true;
}

//bool Engine::InitializeDemoCube() {
//	if (!m_textureShader.LoadFromFiles(
//		GetAssetPath("shaders/textured_cube.vert"),
//		GetAssetPath("shaders/textured_cube.frag"))) {
//		return false;
//	}
//
//	if (!m_cubeTexture.LoadFromFile(GetAssetPath("textures/checker.png"))) {
//		return false;
//	}
//
//	const std::vector<Vertex> vertices = {
//		{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
//		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
//		{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
//
//		{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
//		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
//		{{-0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//		{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
//
//		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
//		{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f}},
//		{{-0.5f,  0.5f,  0.5f}, {1.0f, 1.0f}},
//		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
//
//		{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f}},
//		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
//		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//		{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f}},
//
//		{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f}},
//		{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f}},
//		{{ 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f}},
//		{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f}},
//
//		{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}},
//		{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}},
//		{{ 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f}},
//		{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f}},
//	};
//
//	const std::vector<unsigned int> indices = {
//		 0,  1,  2,  2,  3,  0,
//		 4,  5,  6,  6,  7,  4,
//		 8,  9, 10, 10, 11,  8,
//		12, 13, 14, 14, 15, 12,
//		16, 17, 18, 18, 19, 16,
//		20, 21, 22, 22, 23, 20,
//	};
//
//	if (!m_cubeMesh.Create(vertices, indices)) {
//		return false;
//	}
//
//	m_redMaterial.SetShader(&m_textureShader);
//	m_redMaterial.SetTexture(&m_cubeTexture);
//	m_redMaterial.SetColor({ 1.0f, 0.55f, 0.55f, 1.0f });
//
//	m_greenMaterial.SetShader(&m_textureShader);
//	m_greenMaterial.SetTexture(&m_cubeTexture);
//	m_greenMaterial.SetColor({ 0.55f, 1.0f, 0.55f, 1.0f });
//
//	m_blueMaterial.SetShader(&m_textureShader);
//	m_blueMaterial.SetTexture(&m_cubeTexture);
//	m_blueMaterial.SetColor({ 0.55f, 0.65f, 1.0f, 1.0f });
//
//	m_renderObjects = {
//		{ &m_cubeMesh, &m_redMaterial, glm::translate(glm::mat4(1.0f), glm::vec3(-1.5f, 0.0f, 0.0f)) },
//		{ &m_cubeMesh, &m_greenMaterial, glm::mat4(1.0f) },
//		{ &m_cubeMesh, &m_blueMaterial, glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.0f, 0.0f)) },
//	};
//
//	return true;
//}

void Engine::Run() {
	while (m_isRunning && !m_window->ShouldClose()) {
		glfwPollEvents();
		m_time.Tick();

		m_debugGui.BeginFrame();

		ProcessInput();
		Update();
		Render();

		m_debugGui.EndFrame();
		m_window->SwapBuffers(); // バックバッファーをフロントバッファーに変更
	}
}

void Engine::Shutdown() {
	m_debugGui.Shutdown();
	m_modelViewerObjects.clear();

	m_groundMesh.Destroy();
	m_cubeMesh.Destroy();
	m_groundTexture.Destroy();
	m_cubeTexture.Destroy();
	m_modelTexture.Destroy();
	m_textureShader.Destroy();
	m_demoModel.Destroy();

	m_window.reset();
	glfwTerminate(); // GLFWの終了

	m_isRunning = false;
}

void Engine::ProcessInput() {
	GLFWwindow* nativeWindow = m_window->GetNativeHandle();

	// 指定したキーが押されているかを調べる
	if (glfwGetKey(nativeWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		m_window->SetShouldClose(true); // ウィンドウを閉じるべきというフラグを出す
	}

	const bool shouldUseCameraMouse =
		glfwGetMouseButton(nativeWindow, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	if (shouldUseCameraMouse != m_isCameraMouseActive) {
		m_isCameraMouseActive = shouldUseCameraMouse;
		glfwSetInputMode(
			nativeWindow,
			GLFW_CURSOR,
			m_isCameraMouseActive ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL
		);
		m_isFirstMouseMove = true;
	}

	if (!m_isCameraMouseActive) {
		return;
	}

	const float deltaTime = static_cast<float>(m_time.GetDeltaTime());

	if (glfwGetKey(nativeWindow, GLFW_KEY_W) == GLFW_PRESS) {
		m_camera.ProcessKeyboard(CameraMovement::Forward, deltaTime);
	}
	if (glfwGetKey(nativeWindow, GLFW_KEY_S) == GLFW_PRESS) {
		m_camera.ProcessKeyboard(CameraMovement::Backward, deltaTime);
	}
	if (glfwGetKey(nativeWindow, GLFW_KEY_A) == GLFW_PRESS) {
		m_camera.ProcessKeyboard(CameraMovement::Left, deltaTime);
	}
	if (glfwGetKey(nativeWindow, GLFW_KEY_D) == GLFW_PRESS) {
		m_camera.ProcessKeyboard(CameraMovement::Right, deltaTime);
	}

	double mouseX = 0.0;
	double mouseY = 0.0;
	glfwGetCursorPos(nativeWindow, &mouseX, &mouseY);

	if (m_isFirstMouseMove) {
		m_lastMouseX = mouseX;
		m_lastMouseY = mouseY;
		m_isFirstMouseMove = false;
	}

	const float xOffset = static_cast<float>(mouseX - m_lastMouseX);
	const float yOffset = static_cast<float>(m_lastMouseY - mouseY);

	m_lastMouseX = mouseX;
	m_lastMouseY = mouseY;

	m_camera.ProcessMouseMovement(xOffset, yOffset);
}

void Engine::Update() {

}

void Engine::Render() {
	Renderer::Clear();

	int framebufferWidth = 1;
	int framebufferHeight = 1;
	glfwGetFramebufferSize(m_window->GetNativeHandle(), &framebufferWidth, &framebufferHeight);

	if (framebufferHeight <= 0) {
		framebufferHeight = 1;
	}

	const float aspectRatio =
		static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);

	for (ModelViewerObject& object : m_modelViewerObjects) {
		DrawModelViewerObject(object, aspectRatio);
	}

	std::vector<DebugObjectControls> debugObjects;
	debugObjects.reserve(m_modelViewerObjects.size());

	for (ModelViewerObject& object : m_modelViewerObjects) {
		debugObjects.push_back({
			object.name.c_str(),
			&object.position,
			&object.rotation,
			&object.scale,
			&object.materialColor
		});
	}

	m_debugGui.Draw(
		m_time.GetFPS(),
		m_time.GetDeltaTime(),
		m_isCameraMouseActive,
		debugObjects,
		m_lighting
	);
}

void Engine::DrawModelViewerObject(ModelViewerObject& object, float aspectRatio) {
	const glm::mat4 transform = BuildTransform(object);

	if (object.mesh != nullptr && object.material != nullptr) {
		object.material->SetColor(object.materialColor);
		Renderer::DrawMesh(*object.mesh, *object.material, m_camera, transform, aspectRatio, m_lighting);
	}

	if (object.model != nullptr) {
		object.model->SetMaterialColor(object.materialColor);
		object.model->Draw(m_camera, transform, aspectRatio, m_lighting);
	}
}

glm::mat4 Engine::BuildTransform(const ModelViewerObject& object) const {
	glm::mat4 transform = glm::translate(glm::mat4(1.0f), object.position);
	transform = glm::rotate(transform, glm::radians(object.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(object.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(object.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::scale(transform, object.scale);

	return transform;
}
