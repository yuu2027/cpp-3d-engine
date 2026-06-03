# pragma once

#include <string>

using namespace std;

struct GLFWwindow;

class Window {
public:
	Window() = default;
	~Window();

	bool Create(const string& title, int width, int height);

	bool ShouldClose() const;
	void SetShouldClose(bool shouldClose);
	void SwapBuffers();

	GLFWwindow* GetNativeHandle() const;

private:
	GLFWwindow* m_window = nullptr;
};