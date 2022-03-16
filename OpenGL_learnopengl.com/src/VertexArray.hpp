#pragma once
#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray
{
private:
	// the unsigned int thing for OpenGL
	unsigned int m_RendererID;

public:
	VertexArray();
	~VertexArray();

	// VertexBuffer + respective Layout -> VertexArray
	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

