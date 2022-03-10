#pragma once


class VertexBuffer
{
private:
	unsigned int m_RendererID;

public:
	VertexBuffer(const void *data, unsigned int bytes);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

};
