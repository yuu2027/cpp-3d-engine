#pragma once

#include <memory>
#include <string>

#include "Engine/Time.h"
#include "Platform/Window.h"

using namespace std;

class Engine {
public:
	bool Initialize(const string& title, int width, int height);
	void Run();
	void Shutdown();

private:
	void ProcessInput();
	void Update();
	void Render();

	unique_ptr<Window> m_window;
	Time m_time;
	bool m_isRunning = false;
};