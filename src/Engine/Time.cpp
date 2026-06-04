#include "Engine/Time.h"

Time::Time()
	: m_lastTime(Clock::now())
{
}

void Time::Tick() {
	const auto currentTime = Clock::now(); // 現在時刻を取得
	const chrono::duration<double> elapsed = currentTime - m_lastTime; // 前回との時間差から経過時間を求める

	m_lastTime = currentTime;
	m_deltaTime = elapsed.count(); // 経過時間を
	m_totalTime += m_deltaTime;

	m_accumulatedTime += m_deltaTime;
	++m_frameCount;

	// 1秒経過したらFPSを計算
	if (m_accumulatedTime >= 1.0) {
		m_fps = static_cast<double>(m_frameCount) / m_accumulatedTime; // FPS計算
		m_accumulatedTime = 0.0;
		m_frameCount = 0;
		m_oneSecondTick = true;
	}
}

double Time::GetDeltaTime() const {
	return m_deltaTime;
}

double Time::GetTotalTime() const {
	return m_totalTime;
}

double Time::GetFPS() const {
	return m_fps;
}

// 1秒消費したかを判定
bool Time::ConsumeOneSecondTick() {
	const bool result = m_oneSecondTick; // 1秒経過していたらtrue
	m_oneSecondTick = false;
	return result;
}