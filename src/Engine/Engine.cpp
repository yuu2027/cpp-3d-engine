#include "Engine/Engine.h"

#include <cmath>
# include <iostream>
#include <string>

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

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

	if (!InitializeDemoTriangle()) {
		return false;
	}

	Renderer::SetClearColor(0.02f, 0.02f, 0.025f, 1.0f);
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
	m_triangleVertexBuffer.Destroy();
	m_triangleVertexArray.Destroy();
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
}

void Engine::Update() {

}

void Engine::Render() {
	Renderer::Clear();

	const float time = static_cast<float>(m_time.GetTotalTime());
	const float red = (std::sin(time) + 1.0f) * 0.5f;
	const float green = (std::sin(time + 2.0f) + 1.0f) * 0.5f;
	const float blue = (std::sin(time + 4.0f) + 1.0f) * 0.5f;

	m_basicShader.Use();
	m_basicShader.SetFloat4("uColor", red, green, blue, 1.0f);

	Renderer::DrawArrays(m_triangleVertexArray, m_triangleVertexCount);
}