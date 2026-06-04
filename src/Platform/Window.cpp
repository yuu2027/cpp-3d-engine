#include "Platform/Window.h"

#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

// Windowオブジェクトが破棄されるときに自動で呼ばれる関数
Window::~Window() {
	if (m_window != nullptr) {
		glfwDestroyWindow(m_window); // ウィンドウを破棄
		m_window = nullptr;
	}
}

bool Window::Create(const string& title, int width, int height) { 
	if (!glfwInit()) { // GLFWの開始
		cerr << "Failed to initialize GLFW.\n"; // 標準エラー出力
		return false;
	}

	// 作成するウィンドウやOpenGLコンテキストの設定を指定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // OpenGLのメジャーバージョンを指定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // OpenGLのプロファイルを指定。Core Profileを使うという意味

	m_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

	if (m_window == nullptr) {
		cerr << "Failed to create GLFW window.\n"; // 標準エラー出力
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window); // このウィンドウのOpenGLコンテキストを、現在のスレッドで使えるようにする
	glfwSwapInterval(1);

	//glViewport(0, 0, width, height); // OpenGLの描画領域を設定

	// ウィンドウの描画領域サイズが変わったときに呼ばれる関数を登録
	// ウィンドウサイズが変わったら、OpenGLの描画範囲も新しいサイズに合わせる
	//glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow*, int framebufferWidth, int framebufferHeight) { glViewport(0, 0, framebufferWidth, framebufferHeight);});
	
	return true;
}

bool Window::ShouldClose() const {
	return glfwWindowShouldClose(m_window); // ウィンドウが閉じるべき状態かどうかを取得
}

void Window::SetShouldClose(bool shouldClose) {
	glfwSetWindowShouldClose(m_window, shouldClose ? GLFW_TRUE : GLFW_TRUE); // ウィンドウを閉じるべきかどうかのフラグを自分で設定
}

// 画結果を画面に表示
void Window::SwapBuffers() {
	glfwSwapBuffers(m_window);
}

GLFWwindow* Window::GetNativeHandle() const {
	return m_window;
}