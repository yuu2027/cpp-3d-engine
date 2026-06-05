#include  "Engine/Engine.h"
#include "Rendering/Shader.h"

int main() {
	Engine engine;

	if (!engine.Initialize("Cpp3DEngine - Phase7 Model Viewer", 1280, 720)) {
		return 1; // エラー終了
	}

	engine.Run();
	engine.Shutdown();

	return 0; // 正常終了
}
