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

	if (!InitializeDemoModel()) {
		return false;
	}

	Renderer::SetClearColor(0.02f, 0.02f, 0.025f, 1.0f);
	Renderer::EnableDepthTest(true);

	glfwSetInputMode(m_window->GetNativeHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

bool Engine::InitializeDemoModel() {
	if (!m_textureShader.LoadFromFiles(
		GetAssetPath("shaders/textured_cube.vert"),
		GetAssetPath("shaders/textured_cube.frag"))) {
		return false;
	}

	if (!m_cubeTexture.LoadFromFile(GetAssetPath("textures/phase6_cube_uv_checker.png"))) {
		return false;
	}

	if (!m_modelLoader.LoadObj(
		GetAssetPath("models/phase6_unit_cube.obj"),
		m_demoModel,
		&m_textureShader,
		&m_cubeTexture)) {
		return false;
	}

	m_demoModelTransform =
		glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.5f)) *
		glm::scale(glm::mat4(1.0f), glm::vec3(1.5f));

	return true;
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
		m_time.Tick();

		ProcessInput();
		Update();
		Render();

		m_window->SwapBuffers(); // バックバッファーをフロントバッファーに変更
		glfwPollEvents();

		if (m_time.ConsumeOneSecondTick()) {
			cout << "FPS: " << m_time.GetFPS() << " | DeltaTime: " << m_time.GetDeltaTime() << "s\n";
		}
	}
}

void Engine::Shutdown() {
	m_renderObjects.clear();

	m_cubeMesh.Destroy();
	m_cubeTexture.Destroy();
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

	for (const RenderObject& object : m_renderObjects) {
		if (object.mesh == nullptr || object.material == nullptr) {
			continue;
		}

		Renderer::DrawMesh(
			*object.mesh,
			*object.material,
			m_camera,
			object.model,
			aspectRatio
		);
	}

	m_demoModel.Draw(m_camera, m_demoModelTransform, aspectRatio);
}