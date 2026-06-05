#pragma once

#include <string>

using namespace std;

class Model;
class Shader;
class Texture2D;

class ModelLoader {
public:
    bool LoadObj(const std::string& path, Model& model, Shader* shader, Texture2D* fallbackTexture);

private:
    std::string GetDirectory(const std::string& path) const;
};