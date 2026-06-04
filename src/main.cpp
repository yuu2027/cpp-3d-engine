#include  "Engine/Engine.h"

int main() {
	Engine engine;

	if (!engine.Initialize("Cpp3DEngin - Phase 1", 1280, 720)) {
		return 1; // エラー終了
	}

	engine.Run();
	engine.Shutdown();

	return 0; // 正常終了
}