# C++ 3Dゲームエンジン開発仕様書

## 0. 本書の位置づけ

本書は、C++を使用して3Dゲームエンジンを開発するための仕様書である。  
対象範囲は、**環境構築**から、**中間ゴールである3Dモデルビューア完成**、さらに**最終完成形である自作エンジン上で動く3Dシューティングゲーム完成**までとする。

ユーザー指定の「最終慣性系」は、文脈上「最終完成形」と解釈している。

---

## 1. プロジェクト概要

### 1.1 プロジェクト名

`Cpp3DEngine`

### 1.2 開発目的

C++で3Dゲームエンジンの基礎機能を自作し、最終的にそのエンジン上で簡単な3Dシューティングゲームを制作する。

本プロジェクトでは、単にゲームを作るのではなく、以下を理解・実装することを重視する。

- ウィンドウ生成
- OpenGLによる3D描画
- シェーダーによる色変更
- テクスチャ描画
- 3Dモデル読み込み
- カメラ制御
- ライト表現
- GameObject / Component構造
- シーン管理
- 入力処理
- 当たり判定
- 簡易ゲームロジック

### 1.3 開発方針

最初からUnityやUnreal Engineのような大規模エンジンを目指さない。  
まずはコードベースで扱える軽量な3Dゲームエンジンを作り、段階的に機能を追加する。

---

## 2. 最終完成形

### 2.1 最終ゴール

最終的には、以下の説明ができる状態を目指す。

> C++とOpenGLを用いて3Dゲームエンジンを自作し、シェーダー、テクスチャ、3Dモデル読み込み、カメラ、GameObject / Component、シーン管理、当たり判定を実装した。さらに、そのエンジン上で簡単な3Dシューティングゲームを制作した。

### 2.2 最終的に作るゲーム

ジャンル：簡易3Dシューティングゲーム

#### ゲーム内容

- プレイヤーをWASDで移動できる
- マウスで視点を操作できる
- 左クリックで弾を発射できる
- 敵がステージ内に配置または出現する
- 弾が敵に当たると敵を倒せる
- 敵を倒すとスコアが加算される
- プレイヤーにはHPがある
- HPが0になるとゲームオーバー
- 全敵撃破または目標到達でゲームクリア

---

## 3. 技術選定

### 3.1 採用技術一覧

| 分類 | 採用技術 | 用途 |
|---|---|---|
| 言語 | C++20 | エンジン本体の実装 |
| IDE | Visual Studio 2022 | C++開発環境 |
| ビルド管理 | CMake | プロジェクト構成・ビルド管理 |
| パッケージ管理 | vcpkg | 外部ライブラリ管理 |
| 描画API | OpenGL 3.3 Core | 3D描画 |
| ウィンドウ / 入力 | GLFW | ウィンドウ生成、OpenGLコンテキスト、入力処理 |
| OpenGL関数ロード | GLAD | OpenGL関数の読み込み |
| 数学ライブラリ | GLM | Vector、Matrix、Quaternion |
| 画像読み込み | stb_image | PNG / JPGテクスチャ読み込み |
| 3Dモデル読み込み | Assimp | OBJ / glTFモデル読み込み |
| デバッグGUI | Dear ImGui | FPS表示、Transform編集、Material確認 |
| 音声 | miniaudio | BGM / SE再生 |
| バージョン管理 | Git / GitHub | ソース管理、ポートフォリオ公開 |

### 3.2 OpenGLを選ぶ理由

本プロジェクトの目的は、低レベルGPU制御そのものではなく、3Dゲームエンジンの基礎構造を学ぶことである。

そのため、DirectX 12やVulkanのような低レベルAPIではなく、比較的実装量を抑えながら3D描画を学べるOpenGLを採用する。

---

## 4. 環境構築仕様

### 4.1 想定環境

| 項目 | 内容 |
|---|---|
| OS | Windows 10 / 11 |
| IDE | Visual Studio 2022 |
| コンパイラ | MSVC |
| C++規格 | C++20 |
| ビルド | CMake |
| パッケージ管理 | vcpkg |

### 4.2 インストールするもの

以下をインストールする。

1. Visual Studio 2022
   - ワークロード：`C++によるデスクトップ開発`
2. Git
3. CMake
4. vcpkg
5. 任意：Visual Studio Code

### 4.3 vcpkgセットアップ例

```bash
cd C:\dev
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
```

環境変数またはCMake Presetsでvcpkg toolchainを指定する。

```bash
-DCMAKE_TOOLCHAIN_FILE=C:/dev/vcpkg/scripts/buildsystems/vcpkg.cmake
```

### 4.4 使用ライブラリの導入例

```bash
vcpkg install glfw3:x64-windows
vcpkg install glad:x64-windows
vcpkg install glm:x64-windows
vcpkg install assimp:x64-windows
vcpkg install imgui[glfw-binding,opengl3-binding]:x64-windows
vcpkg install miniaudio:x64-windows
```

`stb_image`はvcpkg経由または単一ヘッダを直接配置する。最初は`external/stb/stb_image.h`としてプロジェクトに含めてもよい。

---

## 5. 推奨フォルダ構成

```txt
Cpp3DEngine/
├─ CMakeLists.txt
├─ CMakePresets.json
├─ README.md
├─ assets/
│  ├─ models/
│  ├─ textures/
│  ├─ shaders/
│  └─ audio/
├─ external/
│  └─ stb/
├─ src/
│  ├─ main.cpp
│  ├─ Engine/
│  │  ├─ Engine.h
│  │  ├─ Engine.cpp
│  │  ├─ Time.h
│  │  └─ Time.cpp
│  ├─ Platform/
│  │  ├─ Window.h
│  │  └─ Window.cpp
│  ├─ Rendering/
│  │  ├─ Renderer.h
│  │  ├─ Renderer.cpp
│  │  ├─ Shader.h
│  │  ├─ Shader.cpp
│  │  ├─ Texture2D.h
│  │  ├─ Texture2D.cpp
│  │  ├─ Mesh.h
│  │  ├─ Mesh.cpp
│  │  ├─ Model.h
│  │  ├─ Model.cpp
│  │  ├─ ModelLoader.h
│  │  ├─ ModelLoader.cpp
│  │  ├─ Material.h
│  │  ├─ Material.cpp
│  │  ├─ Camera.h
│  │  └─ Camera.cpp
│  ├─ Scene/
│  │  ├─ Scene.h
│  │  ├─ Scene.cpp
│  │  ├─ SceneManager.h
│  │  ├─ SceneManager.cpp
│  │  ├─ GameObject.h
│  │  ├─ GameObject.cpp
│  │  ├─ Component.h
│  │  └─ Component.cpp
│  ├─ Components/
│  │  ├─ TransformComponent.h
│  │  ├─ TransformComponent.cpp
│  │  ├─ ModelRendererComponent.h
│  │  ├─ ModelRendererComponent.cpp
│  │  ├─ CameraComponent.h
│  │  ├─ CameraComponent.cpp
│  │  ├─ ColliderComponent.h
│  │  └─ ColliderComponent.cpp
│  ├─ Input/
│  │  ├─ Input.h
│  │  └─ Input.cpp
│  ├─ Physics/
│  │  ├─ Collision.h
│  │  ├─ Collision.cpp
│  │  ├─ SphereCollider.h
│  │  ├─ AABBCollider.h
│  │  └─ Ray.h
│  ├─ Audio/
│  │  ├─ AudioManager.h
│  │  └─ AudioManager.cpp
│  └─ Game/
│     ├─ PlayerController.h
│     ├─ PlayerController.cpp
│     ├─ EnemyController.h
│     ├─ EnemyController.cpp
│     ├─ Bullet.h
│     ├─ Bullet.cpp
│     ├─ GameScene.h
│     └─ GameScene.cpp
└─ docs/
   ├─ design.md
   └─ progress_log.md
```

---

## 6. CMake基本仕様

### 6.1 最小CMakeLists.txt例

```cmake
cmake_minimum_required(VERSION 3.25)
project(Cpp3DEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cpp3DEngine
    src/main.cpp
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(miniaudio CONFIG REQUIRED)

target_link_libraries(Cpp3DEngine PRIVATE
    glfw
    glad::glad
    glm::glm
    assimp::assimp
    imgui::imgui
    miniaudio::miniaudio
)
```

実際には、`src/`以下のファイルが増えるため、後で`target_sources`またはサブディレクトリ管理に移行する。

---

## 7. 中間ゴール仕様

### 7.1 中間ゴール名

**3Dモデルビューア兼・描画エンジン試作版**

### 7.2 中間ゴールの目的

ゲーム本体を作る前に、3D描画の基礎機能を完成させる。

### 7.3 中間ゴールで実装する機能

| 分類 | 機能 |
|---|---|
| Core | ウィンドウ生成、ゲームループ、終了処理、時間管理 |
| Rendering | 三角形描画、立方体描画、深度テスト |
| Shader | 頂点シェーダー、フラグメントシェーダー、色変更 |
| Texture | PNG / JPG読み込み、UVによるテクスチャ貼り付け |
| Model | OBJモデル読み込み、Mesh生成、法線・UV対応 |
| Camera | WASD移動、マウス視点操作、View / Projection行列 |
| Material | 色、テクスチャ、シェーダーの管理 |
| Light | Directional Light、Ambient Light、Lambert反射 |
| Debug | FPS表示、Transform表示、Material色変更 |

### 7.4 中間ゴールの完成条件

以下を満たせば中間ゴール達成とする。

- ウィンドウが生成される
- OpenGLで三角形を描画できる
- シェーダーのUniformで色を変更できる
- 3D空間に立方体を描画できる
- カメラをWASDとマウスで操作できる
- 画像を読み込んで立方体にテクスチャを貼れる
- OBJモデルを読み込んで表示できる
- モデルに簡易ライトで明暗がつく
- ImGuiでFPS、座標、色を確認できる

### 7.5 中間デモ内容

```txt
Scene: ModelViewerScene

配置物:
- 地面Plane
- テクスチャ付きCube
- OBJモデル 1体
- Directional Light
- Free Camera
- ImGui Debug Panel

操作:
- W / A / S / D: カメラ移動
- Mouse: 視点操作
- ImGui: 色変更、ライト方向変更、FPS確認
```

---

## 8. 最終ゴール仕様

### 8.1 最終ゴール名

**自作3Dエンジン上で動く3Dシューティングゲーム**

### 8.2 最終ゴールで実装する追加機能

| 分類 | 機能 |
|---|---|
| Scene | タイトル、ゲーム、ゲームオーバー、ゲームクリア |
| GameObject | オブジェクト生成、削除、検索、更新、描画 |
| Component | Transform、Renderer、Camera、Collider、Controller |
| Input | キー入力、マウス入力、押した瞬間、押している間 |
| Physics | Sphere、AABB、Raycast、簡易衝突判定 |
| Game | Player、Enemy、Bullet、HP、Score、Spawn |
| UI | HP表示、スコア表示、リザルト表示 |
| Audio | BGM、弾発射SE、撃破SE、ダメージSE |

### 8.3 最終完成条件

以下を満たせば最終完成とする。

- タイトルからゲームシーンへ遷移できる
- プレイヤーを3D空間で操作できる
- マウスで視点を変更できる
- 弾を発射できる
- 敵を3Dモデルで表示できる
- 弾と敵の当たり判定ができる
- 敵撃破時にスコアが増える
- HPが0になるとゲームオーバーになる
- 条件達成でゲームクリアになる
- READMEに実装機能、操作方法、技術構成を記載する
- GitHubにソースコードを公開できる状態にする
- デモ動画またはスクリーンショットを用意する

---

## 9. 開発フェーズ

### Phase 0：環境構築

#### 実装・作業内容

- Visual Studio 2022を導入
- Gitを導入
- CMakeを導入
- vcpkgを導入
- GitHubリポジトリを作成
- CMakeプロジェクトを作成

#### 完成条件

- 空のC++プログラムをCMakeでビルドできる
- GitHubへpushできる

---

### Phase 1：ウィンドウとゲームループ

#### 実装内容

- GLFW初期化
- Windowクラス作成
- Engineクラス作成
- メインループ作成
- DeltaTime計測

#### 完成条件

- 黒いウィンドウが表示される
- ESCまたは閉じるボタンで終了できる
- FPSまたはDeltaTimeをログ出力できる

---

### Phase 2：三角形描画とShader

#### 実装内容

- GLAD初期化
- VAO / VBO作成
- Shaderクラス作成
- Vertex Shader作成
- Fragment Shader作成
- Uniformによる色変更

#### 完成条件

- 三角形が表示される
- C++側から色を変更できる

---

### Phase 3：3D描画とCamera

#### 実装内容

- 立方体メッシュ作成
- EBO対応
- 深度テスト有効化
- GLMによるModel / View / Projection行列
- Cameraクラス作成
- WASD移動
- マウス視点操作

#### 完成条件

- 立方体が3D空間に表示される
- カメラを動かして立方体を見回せる

---

### Phase 4：Texture

#### 実装内容

- stb_image導入
- Texture2Dクラス作成
- UV座標対応
- テクスチャ用Shader作成

#### 完成条件

- 立方体にPNGまたはJPG画像を貼れる

---

### Phase 5：Mesh / Material整理

#### 実装内容

- Vertex構造体作成
- Meshクラス作成
- Materialクラス作成
- Rendererクラス作成

#### 完成条件

- 描画処理がMesh、Material、Shaderに分離される
- 複数オブジェクトを描画できる

---

### Phase 6：3Dモデル読み込み

#### 実装内容

- Assimp導入
- ModelLoaderクラス作成
- OBJモデル読み込み
- Mesh配列対応
- 法線、UV、インデックス取得

#### 完成条件

- 外部OBJモデルを読み込んで表示できる
- モデルにテクスチャを適用できる

---

### Phase 7：LightとDebug GUI

#### 実装内容

- Directional Light実装
- Ambient Light実装
- Lambert反射実装
- Dear ImGui導入
- FPS表示
- Transform表示
- Material色変更UI

#### 完成条件

- 光の方向によって3Dモデルに明暗がつく
- ImGuiでデバッグ情報を確認できる

ここで中間ゴール達成とする。

---

### Phase 8：GameObject / Component化

#### 実装内容

- GameObjectクラス作成
- Component基底クラス作成
- TransformComponent作成
- ModelRendererComponent作成
- CameraComponent作成
- Sceneクラス作成
- SceneManager作成

#### 完成条件

- GameObjectにComponentを追加して動作を構成できる
- Scene単位でGameObjectを管理できる

---

### Phase 9：Inputとゲーム制御

#### 実装内容

- Inputクラス作成
- キー入力取得
- マウス入力取得
- PlayerController作成
- EnemyController作成
- Bullet作成

#### 完成条件

- プレイヤーが移動できる
- 弾を発射できる
- 敵を配置できる

---

### Phase 10：Collisionとゲームルール

#### 実装内容

- SphereCollider作成
- AABBCollider作成
- CollisionSystem作成
- BulletとEnemyの当たり判定
- PlayerとEnemyの当たり判定
- HP管理
- Score管理
- GameOver / GameClear判定

#### 完成条件

- 弾が敵に当たると敵を倒せる
- HPやスコアが変化する
- 勝敗条件が動作する

---

### Phase 11：UI / Audio / 仕上げ

#### 実装内容

- タイトル画面
- ゲームオーバー画面
- ゲームクリア画面
- HP表示
- スコア表示
- BGM再生
- SE再生
- README作成
- デモ動画作成

#### 完成条件

- 1つのゲームとして開始から終了まで遊べる
- ポートフォリオとして説明できる資料がある

---

## 10. 主要クラス責務

### 10.1 Engine

エンジン全体の起動・更新・描画・終了を管理する。

責務：

- 初期化
- メインループ
- Update呼び出し
- Render呼び出し
- 終了処理

---

### 10.2 Window

GLFWを使用してウィンドウを管理する。

責務：

- ウィンドウ生成
- OpenGLコンテキスト作成
- 入力イベント取得
- ウィンドウ終了判定

---

### 10.3 Renderer

描画処理全体を管理する。

責務：

- 画面クリア
- 深度テスト設定
- Mesh描画
- Material適用
- Camera行列適用

---

### 10.4 Shader

GLSLシェーダーを管理する。

責務：

- シェーダーファイル読み込み
- コンパイル
- リンク
- Uniform設定

---

### 10.5 Texture2D

2Dテクスチャを管理する。

責務：

- 画像ファイル読み込み
- OpenGLテクスチャ生成
- Bind / Unbind
- 破棄処理

---

### 10.6 Mesh

頂点データと描画バッファを管理する。

責務：

- 頂点配列
- インデックス配列
- VAO / VBO / EBO生成
- Draw呼び出し

---

### 10.7 Model

複数Meshを持つ3Dモデルを管理する。

責務：

- Meshリスト保持
- Material保持
- モデル単位の描画

---

### 10.8 ModelLoader

外部3Dモデルファイルを読み込む。

責務：

- Assimpによる読み込み
- 頂点変換
- Mesh生成
- テクスチャパス取得

---

### 10.9 Material

オブジェクトの見た目を管理する。

責務：

- Shader参照
- 色情報
- Texture参照
- ライト用パラメータ

---

### 10.10 Camera

3D空間を見るためのカメラを管理する。

責務：

- View行列生成
- Projection行列生成
- 位置・向き管理
- マウス視点操作

---

### 10.11 GameObject

シーン内の1つのオブジェクトを表す。

責務：

- 名前管理
- Component保持
- 有効 / 無効状態
- Update / Render委譲

---

### 10.12 Component

GameObjectに追加される機能の基底クラス。

責務：

- 所有GameObject参照
- Update仮想関数
- Render仮想関数

---

### 10.13 Scene

GameObject群を管理する。

責務：

- GameObject生成
- GameObject削除
- Update
- Render

---

### 10.14 SceneManager

現在のSceneとシーン遷移を管理する。

責務：

- 現在シーン保持
- LoadScene
- シーン切り替え

---

### 10.15 CollisionSystem

簡易当たり判定を管理する。

責務：

- Sphere vs Sphere
- AABB vs AABB
- Raycast
- 衝突イベント通知

---

## 11. 実装上のルール

### 11.1 C++設計ルール

- 生ポインタの所有は避け、`std::unique_ptr`を基本にする
- 共有所有が必要な場合のみ`std::shared_ptr`を使う
- OpenGLリソースはRAIIで管理する
- コピー禁止が必要なクラスは`delete`する
- `const`を適切に使う
- 1クラス1責務を意識する

### 11.2 命名規則

| 対象 | 規則 | 例 |
|---|---|---|
| クラス | PascalCase | `GameObject` |
| 関数 | PascalCase | `LoadScene()` |
| 変数 | camelCase | `deltaTime` |
| メンバ変数 | m_接頭辞またはcamelCase | `m_position` |
| 定数 | PascalCaseまたはUpperSnake | `MaxEnemyCount` |

命名規則は途中で混在させないことを優先する。

---

## 12. Git運用

### 12.1 ブランチ方針

| ブランチ | 用途 |
|---|---|
| main | 安定版 |
| develop | 開発統合 |
| feature/window | ウィンドウ実装 |
| feature/rendering | 描画実装 |
| feature/model-loader | モデル読み込み実装 |
| feature/gameplay | ゲーム機能実装 |

### 12.2 コミット例

```txt
feat: add GLFW window initialization
feat: add shader class
feat: render textured cube
feat: load OBJ model with assimp
refactor: split mesh and material classes
fix: correct camera mouse rotation
```

---

## 13. ポートフォリオ用成果物

最終的に以下を用意する。

- GitHubリポジトリ
- README.md
- スクリーンショット
- デモ動画
- 実装機能一覧
- 技術選定理由
- クラス構成図
- 中間ゴールと最終ゴールの比較

READMEには最低限以下を書く。

```md
# Cpp3DEngine

## 概要
C++とOpenGLで作成した軽量3Dゲームエンジンです。

## 実装機能
- OpenGL描画
- GLSLシェーダー
- テクスチャ描画
- OBJモデル読み込み
- カメラ制御
- GameObject / Component
- 当たり判定
- 3Dシューティングゲーム

## 使用技術
- C++20
- OpenGL
- GLFW
- GLAD
- GLM
- stb_image
- Assimp
- Dear ImGui
- miniaudio

## 操作方法
- W/A/S/D: 移動
- Mouse: 視点操作
- Left Click: 弾発射
- ESC: 終了
```

---

## 14. 開発優先順位

優先順位は以下とする。

```txt
1. 環境構築
2. ウィンドウ生成
3. 三角形描画
4. シェーダーで色変更
5. 立方体描画
6. カメラ実装
7. テクスチャ描画
8. Mesh / Material整理
9. OBJモデル読み込み
10. ライト実装
11. ImGuiデバッグ表示
12. GameObject / Component化
13. Scene管理
14. Input管理
15. 当たり判定
16. Player / Enemy / Bullet実装
17. GameOver / GameClear
18. UI / Audio
19. README / デモ動画
```

---

## 15. スケジュール目安

| 期間 | 目標 |
|---|---|
| 1週目 | 環境構築、CMake、GLFWウィンドウ |
| 2週目 | 三角形描画、Shaderクラス |
| 3週目 | 立方体描画、Camera |
| 4週目 | Texture、Material |
| 5週目 | Mesh、Model、AssimpによるOBJ読み込み |
| 6週目 | Light、ImGui、中間デモ完成 |
| 7週目 | GameObject / Component |
| 8週目 | Scene管理、Input管理 |
| 9週目 | Player、Enemy、Bullet |
| 10週目 | Collision、HP、Score |
| 11週目 | UI、Audio、ゲーム完成 |
| 12週目 | README、動画、リファクタリング |

---

## 16. リスクと対策

| リスク | 内容 | 対策 |
|---|---|---|
| OpenGL初期化で詰まる | GLAD / GLFW / CMake設定が複雑 | 最初は最小構成で三角形だけ表示する |
| モデル読み込みで詰まる | Assimpのデータ構造が複雑 | 最初はOBJのみ対応する |
| 設計が肥大化する | ECSやエディタまで作ろうとする | GameObject / Componentに限定する |
| 完成しない | 機能追加しすぎる | 中間ゴールを先に達成する |
| バグ原因が分からない | 描画は失敗してもエラーが見えにくい | ログ出力とOpenGLエラーチェックを入れる |

---

## 17. 後回しにする機能

以下は最初から実装しない。

- PBR
- シャドウマップ
- スケルタルアニメーション
- Vulkan / DirectX 12対応
- 本格的な物理エンジン
- ノードベースエディタ
- ビジュアルスクリプティング
- ネットワーク対戦
- マルチスレッドレンダリング

理由は、最初の目的が「エンジンの基礎構造を完成させること」だからである。

---

## 18. 参考資料

- CMake: https://cmake.org/
- CMake Tutorial: https://cmake.org/cmake/help/latest/guide/tutorial/index.html
- vcpkg: https://vcpkg.io/
- vcpkg Overview: https://learn.microsoft.com/en-us/vcpkg/get_started/overview
- GLFW: https://www.glfw.org/
- GLFW Documentation: https://www.glfw.org/docs/latest/
- GLM: https://glm.g-truc.net/
- stb: https://github.com/nothings/stb
- Assimp: https://github.com/assimp/assimp
- Dear ImGui: https://github.com/ocornut/imgui
- miniaudio: https://miniaud.io/

---

## 19. まとめ

本プロジェクトは、以下の順番で進める。

1. **環境構築**
2. **OpenGL描画の基礎**
3. **シェーダーで色変更**
4. **テクスチャ描画**
5. **3Dモデル読み込み**
6. **中間ゴール：3Dモデルビューア完成**
7. **GameObject / Component化**
8. **Scene、Input、Collision追加**
9. **最終ゴール：3Dシューティングゲーム完成**

最重要方針は、最初から大規模化しないことである。  
まずは3Dモデルビューアを完成させ、その後にゲームエンジンとして必要な構造を追加していく。
