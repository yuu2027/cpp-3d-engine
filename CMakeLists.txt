cmake_minimum_required (VERSION 3.25) # CMakeの最低バージョン

# プロジェクト名をCpp3DEngineにする
# LANGUAGES CXX:このプロジェクトではC++を使用
project(Cpp3DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)          # 使用するC++の規格をC++20に設定
set(CMAKE_CXX_STANDARD_REQUIRED ON) # 指定したC++規格を必須にする

# Cpp3DEngineという名前の実行ファイルを作る設定
add_executable(Cpp3DEngine
    src/main.cpp
    src/Engine/Engine.cpp
    src/Engine/Time.cpp
    src/Platform/Window.cpp
    src/Rendering/Shader.cpp
    src/Rendering/VertexArray.cpp
    src/Rendering/VertexBuffer.cpp
    src/Rendering/Renderer.cpp
    src/Rendering/IndexBuffer.cpp
    src/Rendering/Camera.cpp
    src/Rendering/Texture2D.cpp
    src/Rendering/Mesh.cpp
    src/Rendering/Material.cpp
    src/Rendering/Model.cpp
    src/Rendering/ModelLoader.cpp
    src/Debug/DebugGui.cpp
)

# Cpp3DEngineのインクルードパスにsrcを追加
# PRIVATE:このインクルード設定はCpp3DEngineの内部だけで使う
target_include_directories(Cpp3DEngine PRIVATE 
    src
    ${Stb_INCLUDE_DIR}
)

target_compile_definitions(Cpp3DEngine PRIVATE
    CPP3D_ASSET_DIR="${CMAKE_SOURCE_DIR}/assets"
)

target_compile_options(Cpp3DEngine PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/utf-8>
)

# GLFWとOpenGLを探す
# REQUIRED:GLFWが見つからなければエラーにする
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# 自分のプログラムと外部ライブラリを結合する
# C++コードでGLFWやOpenGLの関数を使う場合、コンパイルだけでなくリンクも必要
target_link_libraries(Cpp3DEngine PRIVATE
    glfw
    glad::glad
    OpenGL::GL
    glm::glm
    assimp::assimp
    imgui::imgui
)

# Cpp3DEngineというターゲットに対して、追加のプロパティを設定
# 実行ファイル .exeの出力場所を自分で整理
# RUNTIME_OUTPUT_DIRECTORY:.exeなどの実行ファイルをどこに出力するかを決める設定
# CMAKE_BINARY_DIR:CMakeのビルドディレクトリを表す
set_target_properties(Cpp3DEngine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
