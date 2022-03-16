#pragma once


class VertexBuffer
{
private:
	// the unsigned int thing for OpenGL
	unsigned int m_RendererID;

public:
	// data | sizeof(data)
	VertexBuffer(const void *data, unsigned int bytes);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

};
