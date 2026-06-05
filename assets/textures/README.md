# Phase 4 Textures for Cpp3DEngine

These PNG files are intended for Phase 4: Texture.

Recommended first test:
1. Load `uv_checker_512.png` with stb_image.
2. Apply it to a cube.
3. Check UV direction, image flipping, wrapping, and sampling.

Files:
- `uv_checker_512.png`: UV orientation/debug checker.
- `crate_albedo_512.png`: cube/crate material for an obvious textured cube demo.
- `stone_tiles_albedo_512.png`: ground or wall tile.
- `metal_panel_albedo_512.png`: sci-fi/engine panel.
- `target_debug_512.png`: center/UV distortion checking.

Notes:
- Resolution: 512 x 512.
- Format: PNG, RGB.
- Use `GL_RGB` when uploading these textures unless your loader converts to RGBA.
- If stb_image loads with vertical inversion, call `stbi_set_flip_vertically_on_load(true)` before loading.
