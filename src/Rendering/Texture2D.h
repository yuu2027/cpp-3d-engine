#pragma once

#include <string>

using namespace std;

class Texture2D {
public:
    Texture2D() = default;
    ~Texture2D();

    Texture2D(const Texture2D&) = delete;
    Texture2D& operator=(const Texture2D&) = delete;

    bool LoadFromFile(const string& path);
    void Bind(unsigned int slot = 0) const;
    static void Unbind(unsigned int slot = 0);
    void Destroy();

private:
    unsigned int m_id = 0;
    int m_width = 0;
    int m_height = 0;
    int m_channels = 0;
};