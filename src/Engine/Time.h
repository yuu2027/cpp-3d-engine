#pragma once

#include <chrono>

using namespace std;

class Time {
public:
	Time();

	void Tick();

	double GetDeltaTime() const;
	double GetFPS() const;
	bool ConsumeOneSecondTick();

private:
	using Clock = chrono::steady_clock;

	Clock::time_point m_lastTime;
	double m_deltaTime = 0.0;
	double m_accumulatedTime = 0.0;
	double m_fps = 0.0;
	int m_frameCount = 0;
	bool m_oneSecondTick = false;
};