#include "Engine/Time.h"

Time::Time()
	: m_lastTime(Clock::now())
{
}

void Time::Tick() {
	const auto currentTime = Clock::now();
	const chrono::duration<double> elapsed = currentTime - m_lastTime;

	m_lastTime = currentTime;
	m_deltaTime = elapsed.count();

	m_accumulatedTime += m_deltaTime;
	++m_frameCount;

	if (m_accumulatedTime >= 1.0) {
		m_fps = static_cast<double>(m_frameCount) / m_accumulatedTime;
		m_accumulatedTime = 0.0;
		m_frameCount = 0;
		m_oneSecondTick = true;
	}
}

double Time::GetDeltaTime() const {
	return m_deltaTime;
}

double Time::GetFPS() const {
	return m_fps;
}

bool Time::ConsumeOneSecondTick() {
	const bool result = m_oneSecondTick;
	m_oneSecondTick = false;
	return result;
}