#pragma once

#include <cstddef>

class IndexBuffer {
public:
    IndexBuffer() = default;
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&) = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    void Create(const unsigned int* data, std::size_t count);
    void Bind() const;
    static void Unbind();
    void Destroy();

    int GetCount() const;

private:
    unsigned int m_id = 0;
    int m_count = 0;
};