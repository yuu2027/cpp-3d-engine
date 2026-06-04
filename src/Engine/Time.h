#pragma once

#include <chrono> // 標準ライブラリの時間を扱うためのヘッダ

using namespace std;

class Time {
public:
	Time();

	void Tick(); // 1秒経過したか判定し、経過していたらFPSを計算

	double GetDeltaTime() const;
	double GetFPS() const;
	bool ConsumeOneSecondTick(); // 1秒消費したかを判定

private:
	using Clock = chrono::steady_clock; // 単調増加する時計

	Clock::time_point m_lastTime;
	double m_deltaTime = 0.0;
	double m_accumulatedTime = 0.0; // FPS計算用に経過時間をためる変数
	double m_fps = 0.0;
	int m_frameCount = 0; // FPS計算用のフレーム数カウント
	bool m_oneSecondTick = false; // 1秒経過イベントが発生したか
};