#include "Platform/Window.h"

#include <iostream>

#include <GLFW/glfw3.h>

Window::~Window() {
	if (m_window != nullptr) {
		glfwDestroyWindow(m_window);
		m_window = nullptr;
	}
}

bool Window::Create(const string& title, int width, int height) {
	if (!glfwInit()) {
		cerr << "Failed to initialize GLFW.\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (m_window == nullptr) {
		cerr << "Failed to create GLFW window.\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(1);

	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int framebufferWidth, int framebufferHeight) { glViewport(0, 0, framebufferWidth, framebufferHeight);});
	
	return true;
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(m_window);
}

void Window::SetShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(m_window, shouldClose ? GLFW_TRUE : GLFW_FALSE);
}

void Window::SwapBuffers() {
	glfwSwapBuffers(m_window);
}

GLFWwindow* Window::GetNativeHandle() const {
	return m_window;
}