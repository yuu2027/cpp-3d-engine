#include "Engine/Engine.h"

# include <iostream>

#include <GLFW/glfw3.h>

bool Engine::Initialize(const string& title, int width, int height) {
	m_window = make_unique<Window>();

	if (!m_window->Create(title, width, height)) {
		cerr << "Failed to create window.\n";
		return false;
	}

	glClearColor(0.02f, 0.02f, 0.025f, 1.0f);
	m_isRunning = true;

	return true;
}

void Engine::Run() {
	while (m_isRunning && !m_window->ShouldClose()) {
		m_time.Tick();

		ProcessInput();
		Update();
		Render();

		m_window->SwapBuffers();
		glfwPollEvents();

		if (m_time.ConsumeOneSecondTick()) {
			cout << "FPS: " << m_time.GetFPS() << " | DeltaTime: " << m_time.GetDeltaTime() << "s\n";
		}
	}
}

void Engine::Shutdown() {
	m_window.reset();
	glfwTerminate();
}

void Engine::ProcessInput() {
	GLFWwindow* nativeWindow = m_window->GetNativeHandle();

	if (glfwGetKey(nativeWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		m_window->SetShouldClose(true);
	}
}

void Engine::Update() {

}

void Engine::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
}