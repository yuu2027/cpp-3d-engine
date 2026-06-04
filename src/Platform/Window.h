# pragma once

#include <string>

using namespace std;

struct GLFWwindow;

class Window {
public:
	Window() = default;
	~Window();

	bool Create(const string& title, int width, int height);

	bool ShouldClose() const; // ウィンドウが閉じるべき状態かどうかを取得
	void SetShouldClose(bool shouldClose); // ウィンドウ終了のフラグを立てる
	void SwapBuffers(); // // 画結果を画面に表示

	GLFWwindow* GetNativeHandle() const;

private:
	GLFWwindow* m_window = nullptr;
};