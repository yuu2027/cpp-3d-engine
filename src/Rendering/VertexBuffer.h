#pragma once

#include <cstddef>

using namespace std;

class VertexBuffer {
public:
    VertexBuffer() = default;
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&) = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    void Create(const void* data, std::size_t sizeBytes);
    void Bind() const;
    static void Unbind();

    void Destroy();

private:
	unsigned int m_id = 0;
};