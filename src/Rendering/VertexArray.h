#pragma once

using namespace std;

class VertexArray {
public:
	VertexArray() = default;
	~VertexArray();

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator = (const VertexArray&) = delete;

	void Create();
	void Bind() const;
	static void Unbind();

	void SetFloatAttribute(unsigned int index, int componentCount, int strideBytes, const void* offset);
	void Destroy();

private:
	unsigned int m_id = 0;
};