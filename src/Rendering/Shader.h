#pragma once

#include <string>

#include <glm/mat4x4.hpp>

using namespace std;

class Shader {
public:
    Shader() = default;
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    bool LoadFromFiles(const string& vertexPath, const string& fragmentPath);
    void Use() const;
    void SetFloat4(const string& name, float x, float y, float z, float w) const;
    void SetMat4(const string& name, const glm::mat4& value) const;
    void Destroy();

private:
    unsigned int Compile(unsigned int type, const string& source) const;
    string ReadFile(const string& path) const;

    unsigned int m_programId = 0;
};