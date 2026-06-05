#include "Engine/Engine.h"

#include <cmath>
# include <iostream>
#include <string>
#include <iterator>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

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

	if (!InitializeDemoCube()) {
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

bool Engine::InitializeDemoCube() {
	if (!m_basicShader.LoadFromFiles(
		GetAssetPath("shaders/basic_color.vert"),
		GetAssetPath("shaders/basic_color.frag"))) {
		return false;
	}

	const float vertices[] = {
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f,

		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
	};

	const unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4,
		4, 7, 3, 3, 0, 4,
		1, 5, 6, 6, 2, 1,
		3, 2, 6, 6, 7, 3,
		4, 5, 1, 1, 0, 4,
	};

	m_cubeVertexArray.Create();
	m_cubeVertexArray.Bind();

	m_cubeVertexBuffer.Create(vertices, sizeof(vertices));
	m_cubeVertexArray.SetFloatAttribute(0, 3, 3 * sizeof(float), nullptr);

	m_cubeIndexBuffer.Create(indices, std::size(indices));

	VertexBuffer::Unbind();
	VertexArray::Unbind();

	return true;
}

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
	m_cubeIndexBuffer.Destroy();
	m_cubeVertexBuffer.Destroy();
	m_cubeVertexArray.Destroy();
	m_basicShader.Destroy();

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

	const float time = static_cast<float>(m_time.GetTotalTime());
	const float red = (std::sin(time) + 1.0f) * 0.5f;
	const float green = (std::sin(time + 2.0f) + 1.0f) * 0.5f;
	const float blue = (std::sin(time + 4.0f) + 1.0f) * 0.5f;

	int framebufferWidth = 1;
	int framebufferHeight = 1;
	glfwGetFramebufferSize(m_window->GetNativeHandle(), &framebufferWidth, &framebufferHeight);

	if (framebufferHeight <= 0) {
		framebufferHeight = 1;
	}

	const float aspectRatio =
		static_cast<float>(framebufferWidth) / static_cast<float>(framebufferHeight);

	const glm::mat4 model = glm::mat4(1.0f);
	const glm::mat4 view = m_camera.GetViewMatrix();
	const glm::mat4 projection = m_camera.GetProjectionMatrix(aspectRatio);

	m_basicShader.Use();
	m_basicShader.SetMat4("uModel", model);
	m_basicShader.SetMat4("uView", view);
	m_basicShader.SetMat4("uProjection", projection);
	m_basicShader.SetFloat4("uColor", red, green, blue, 1.0f);

	Renderer::DrawIndexed(m_cubeVertexArray, m_cubeIndexBuffer);
}