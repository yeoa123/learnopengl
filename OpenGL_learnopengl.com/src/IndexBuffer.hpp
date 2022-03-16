#pragma once
#include "Renderer.hpp"

class IndexBuffer
{
private:
	// the unsigned int thing for OpenGL
	unsigned int m_RendererID;
	// number of uints (indices) in this buffer
	unsigned int m_Count;

public:
	// data | sizeof(data)
	IndexBuffer(const unsigned* data, unsigned int bytes);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	// get number of uints (indices) in this buffer
	inline unsigned int getCount() const { return m_Count; };
};
